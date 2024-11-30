cmake_minimum_required(VERSION 3.28)
project(viva_ver2 C)

set(CMAKE_C_STANDARD 11)

add_executable(viva_ver2 main.c
        ../viva_test/main.c)
