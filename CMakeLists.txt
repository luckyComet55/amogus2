cmake_minimum_required(VERSION 3.22)
project(amogus2)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(app_client)
add_subdirectory(app_server)
include_directories(app_conf)
