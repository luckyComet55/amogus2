#include "../app_conf/app_conf.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <pthread.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "server.h"

PGconn* DBCONN;
pthread_mutex_t mtx_s_run_;
pthread_mutex_t mtx_output_;
static unsigned char flag_run_ = 0;
CLI_LIST* clients_ = NULL;
char SLOTS[MAX_CLIENTS] = { 0 };

void cliList(CLI_LIST** list) {
    (*list) = (CLI_LIST*) malloc(sizeof (CLI_LIST));
    (*list)->size = 0;
    (*list)->head = NULL;
}

int addNode(struct cli_node** head, CLIENT_T val) {
    while(*head) {
        head = &((*head)->next);
    }
    (*head) = (struct cli_node*) malloc(sizeof (struct cli_node));
    if(*head == NULL) {
        printf("Could not allocate memory\nTerminating\n");
        return 1;
    }
    (*head)->val = val;
    (*head)->next = NULL;
    return 0;
}

void addCli(CLI_LIST** list_, CLIENT_T val_) {
    if (addNode(&((*list_)->head), val_)) {
        exit(1);
    }
    (*list_)->size++;
}

void delCli(CLI_LIST** list_, u_int64 id_rem_) {
    if(!(*list_)->head) {
        return;
    }
    struct cli_node* temp_ = (*list_)->head;
    struct cli_node* temp_hold_ = (*list_)->head;
    while(temp_->val.cli_id_ != id_rem_) {
        if(temp_->next == NULL) {
            printf("No such client\n");
            return;
        }
        temp_hold_ = temp_;
        temp_ = temp_->next;
    }
    pthread_cancel(temp_->val.related_thread_);
    closesocket(temp_->val.related_sock_);
    temp_hold_->next = temp_->next;
    if(temp_ == (*list_)->head) {
        (*list_)->head = (*list_)->head->next;
    }
    free(temp_);
    (*list_)->size--;
}

void cliListDes(CLI_LIST** list_) {
    struct cli_node* temp_ = (*list_)->head;
    while(temp_) {
        temp_ = temp_->next;
        free((*list_)->head);
        (*list_)->head = temp_;
    }
    free(*list_);
}

CLIENT_T* getCli(CLI_LIST* list_, u_int64 id_find_) {
    struct cli_node* temp_ = list_->head;
    while(temp_->val.cli_id_ != id_find_) {
        if(temp_->next == NULL) {
            pthread_mutex_lock(&mtx_output_);
            printf("Could not find such a user\n");
            pthread_mutex_unlock(&mtx_output_);
            return (CLIENT_T*)NULL;
        }
        temp_ = temp_->next;
    }
    return &temp_->val;
}

void cliShowAll(CLI_LIST* list_) {
    printf("Currently handling %llu clients:\n", list_->size);
    struct cli_node* temp = list_->head;
    for (int i = 0; i < list_->size; ++i) {
        printf("ID - %llu\n", temp->val.cli_id_);
        temp = temp->next;
    }
}

void* serverControlInterface(void* args) {
    printf("Supported commands:\n"
           "kill - terminate all threads and shut down server\n"
           "sha - show list of all clients at the moment\n"
           "shu *id* - show client with id *id*\n"
           "delu *id* - terminate thread of client with id *id*\n"
           "exit - exit terminal\n");
    char command_[100];
    char argv[100][2];
    scanf("");
    while(1) {
        pthread_mutex_lock(&mtx_output_);
        printf(">");
        scanf("%s", command_);
        int vals_ = sscanf_s(command_, "%5s %4s\n", argv[0], argv[1]);
        switch (vals_) {
            case 1:
                if(strcmp(argv[0], "kill") == 0) {
                    pthread_mutex_lock(&mtx_s_run_);
                    printf("Process killed but in a very bad way\n");
                    pthread_mutex_unlock(&mtx_s_run_);
                    exit(0);
                }
                if(strcmp(argv[0], "sha") == 0) {
                    cliShowAll(clients_);
                }
                if(strcmp(argv[0], "exit") == 0) {

                }
                break;
            case 2:
                if(strcmp(argv[0], "shu") == 0) {
                    printf("Showing info for user %s\n", argv[1]);
                }
                if(strcmp(argv[0], "delu") == 0) {
                    printf("Deleting user with id %s\n", argv[1]);
                }
                break;
            default:
                printf("Unsupported command\n");
                break;
        }
        pthread_mutex_unlock(&mtx_output_);
    }
}

int receiveData(SOCKET cli_, char* rec_buf_) {
    int rec_stat_ = recv(cli_, rec_buf_, MAX_RECEIVE_LEN, 0);
    if(rec_stat_ <= 0 || rec_stat_ == SOCKET_ERROR) {
        if(pthread_mutex_trylock(&mtx_output_) != EBUSY) {
            printf("Could not receive message\n");
            pthread_mutex_unlock(&mtx_output_);
        }
        return 1;
    }
    rec_buf_[rec_stat_] = '\0';
    printf("Received message: %s\n", rec_buf_);
    return 0;
}

int sendData(SOCKET cli_, char* transmit_buf_) {
    int transmit_stat_ = send(cli_, transmit_buf_, strlen(transmit_buf_), 0);
    if(transmit_stat_ == SOCKET_ERROR || transmit_stat_ < 0) {
        if(pthread_mutex_trylock(&mtx_output_) != EBUSY) {
            printf("Could not transmit message\n");
            pthread_mutex_unlock(&mtx_output_);
        }
        return 1;
    }
    printf("Sent message: %s\n", transmit_buf_);
    return 0;
}

void* cliHandler(void* args) {
    u_int64 this_cli_id_ = (u_int64)args;
    CLIENT_T* this_cli_t_ = getCli(clients_, this_cli_id_);
    SOCKET client_ = this_cli_t_->related_sock_;
    char rec_[MAX_RECEIVE_LEN] = { 0 }, trsm_[MAX_TRANSMIT_LEN] = { 0 };
    char comm_[MAX_COMM_LEN] = { 0 }, data_[2][MAX_COMM_LEN] = { { 0 }, { 0 } };
    char db_req_[MAX_REQ_LEN] = { 0 };
    int flag_;
    if(receiveData(client_, rec_)) {
        flag_ = 1;
    } else {
        sscanf(rec_, "%s %s %s", comm_, data_[0], data_[1]);
        printf("Scanned data: %s %s %s\n", comm_, data_[0], data_[1]);
        if(strcmp(comm_, "USRGT") == 0) {
            sprintf_s(db_req_, MAX_REQ_LEN, "SELECT * FROM users WHERE username = '%s'"
                                            " AND password = '%s';", data_[0], data_[1]);
        } else if(strcmp(comm_, "USRST") == 0) {
            sprintf_s(db_req_, MAX_REQ_LEN, "INSERT INTO users (username, password)"
                                            " VALUES ('%s', '%s');", data_[0], data_[1]);
        }
        printf("Command to the database: %s\n", db_req_);
        PGresult* db_req_res_ = PQexec(DBCONN, db_req_);
        if(PQresultStatus(db_req_res_) != PGRES_COMMAND_OK && PQresultStatus(db_req_res_) != PGRES_TUPLES_OK) {
            printf("Bad DB response\n");
        }
        if(strcmp(comm_, "USRGT") == 0) {
            printf("DB responded in query with: %s\n", PQgetvalue(db_req_res_, 0, 0));
        }
        sprintf_s(trsm_, MAX_TRANSMIT_LEN, "%llu your logging parameters: %s %s", this_cli_id_, data_[0], data_[1]);
        flag_ = sendData(client_, trsm_);
    }
    while(!flag_) {
        if(receiveData(client_, rec_)) {
            flag_ = 1;
        } else {
            sprintf_s(trsm_, MAX_TRANSMIT_LEN, "Echoing: %s", rec_);
            flag_ = sendData(client_, trsm_);
        }
    }
    delCli(&clients_, this_cli_id_);
}

u_int64 getFreeSlot() {
    u_int64 id_ = rand() % MAX_CLIENTS;
    if(SLOTS[id_]) {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!SLOTS[i]) {
                SLOTS[i] = 1;
                return i;
            }
        }
        return -1;
    }
    return id_;
}

int startServer() {
    SOCKET server_, client_;
    SOCKADDR_IN localaddr_, cliaddr_;
    int size;
    server_ = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(server_ == INVALID_SOCKET) {
        printf("Could not create a socket for server\nTerminating\n");
        return 1;
    }
    localaddr_.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    localaddr_.sin_port = htons(SERVER_PORT);
    localaddr_.sin_family = AF_INET;
    if(bind(server_, (SOCKADDR*)&localaddr_, sizeof(localaddr_)) == SOCKET_ERROR) {
        printf("Could not create server with described parameters\nTerminating\n");
        return 1;
    }
    printf("Server is online\n");

    pthread_mutex_init(&mtx_s_run_, NULL);
    pthread_mutex_init(&mtx_output_, NULL);
    //pthread_t com_con_;
    //pthread_create(&com_con_, NULL, serverControlInterface, NULL);
    cliList(&clients_);

    listen(server_, MAX_CLIENTS);
    flag_run_ = 1;
    while(1) {
        pthread_mutex_lock(&mtx_s_run_);
        if(!flag_run_) {
            pthread_mutex_unlock(&mtx_s_run_);
            break;
        }
        pthread_mutex_unlock(&mtx_s_run_);

        size = sizeof (cliaddr_);
        client_ = accept(server_, (SOCKADDR*)&cliaddr_, &size);
        if(client_ == INVALID_SOCKET) {
            printf("Could not accept client\n");
            continue;
        } else {
            printf("Client accepted\n");
        }
        pthread_t cli_trd_;
        u_int64 cli_id__ = getFreeSlot();
        if(cli_id__ == -1) {
            printf("Too many clients\n");
            continue;
        }
        CLIENT_T cli_arg_ = { cli_id__, client_, cli_trd_};
        addCli(&clients_, cli_arg_);
        int stat_ = pthread_create(&cli_trd_, NULL, cliHandler, (void*)cli_id__);
        if(stat_) {
            printf("Could not create a thread\n");
            continue;
        }
        pthread_detach(cli_trd_);
    }
    pthread_mutex_destroy(&mtx_s_run_);
    pthread_mutex_destroy(&mtx_output_);
    cliListDes(&clients_);
    return 0;
}

void breakDBConn(PGconn* conn) {
    PQfinish(conn);
    exit(1);
}

int main(int argc, char* argv[]) {
    WSADATA wsa;
    srand(time(0));
    if(WSAStartup(MAKEWORD(1, 1), &wsa) != 0) {
        printf("WSAStartup finished with error code %d\nTerminating program\n", WSAGetLastError());
        return 1;
    }
    //return startServer();
    int lib_ver = PQlibVersion();
    printf("Version of libpq: %d\n", lib_ver);

    DBCONN = PQconnectdb(""); /* Parse connect data here */
    if(PQstatus(DBCONN) == CONNECTION_BAD) {
        printf("Connection to database failed: %s\n", PQerrorMessage(DBCONN));
        breakDBConn(DBCONN);
    }
    printf("Server version: %d\n", PQserverVersion(DBCONN));

    int server_fin_stat_ = startServer();
    PQfinish(DBCONN);
    return server_fin_stat_;
}