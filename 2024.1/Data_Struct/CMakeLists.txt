cmake_minimum_required(VERSION 3.27)
project(Data_Struct C)

set(CMAKE_C_STANDARD 11)

add_executable(Data_Struct main.c
        bst.c
        calc.c
        bst.h
        calc.h)
