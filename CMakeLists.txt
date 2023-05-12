cmake_minimum_required(VERSION 3.10.1)
project(MYPOPEN)

set(CMAKE_C_STANDARD 11)

add_library(
        mypopen
        include/mypopen.h
        src/mypopen.c)

add_executable(
        mypopen_test 
        test/main.c)

target_link_libraries(
        mypopen_test
        PRIVATE mypopen
)