#pragma comment("wsa2_32.lib")
#include <winsock.h>
#include <pthread.h>
#ifndef SERVER_H
#define SERVER_H

typedef struct client_t {
    u_int64 cli_id_;
    SOCKET related_sock_;
    pthread_t related_thread_;
} CLIENT_T;

typedef struct cli_list {
    size_t size;
    struct cli_node {
        CLIENT_T val;
        struct cli_node* next;
    }* head;
} CLI_LIST;

#endif //SERVER_H
