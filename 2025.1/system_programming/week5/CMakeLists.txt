cmake_minimum_required(VERSION 3.28)
project(week5 C)

set(CMAKE_C_STANDARD 11)

add_executable(week5 main.c
        gpt.c
        scoreAnalyze.c)
