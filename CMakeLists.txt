cmake_minimum_required(VERSION 3.13)
project(intro C)

set(CMAKE_C_STANDARD 11)

add_definitions(-g
                -O2
                -Wall
                -D_REENTRANT)

include_directories(..)
include_directories(../lib)

link_libraries(resolv pthread)
add_link_options(/Users/wangxiaocheng/Codes/UnixNetwork/unpv13e/libunp.a)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/)

#add_executable(hw3-1cli hw3-1cli.c)
#add_executable(hw3-1srv hw3-1srv.c)
#add_executable(hw3-2cli hw3-2cli.c)
#add_executable(hw3-2srv hw3-2srv.c)
#add_executable(hw3-2srv-udp hw3-2srv-udp.c)
#add_executable(hw3-2cli-udp hw3-2cli-udp.c)
add_executable(server server.c)
add_executable(client client.c)
