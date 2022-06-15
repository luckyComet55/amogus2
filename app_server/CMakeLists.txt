cmake_minimum_required(VERSION 3.22)
project(amogus2)

set(CMAKE_CXX_STANDARD 23)

find_package(PostgreSQL REQUIRED)

add_executable(amogus2_server server.c)
target_link_libraries(amogus2_server ws2_32.lib)
target_link_libraries(amogus2_server PostgreSQL::PostgreSQL)
