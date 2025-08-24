cmake_minimum_required(VERSION 4.0)
project(dogwater_16 C)

set(CMAKE_C_STANDARD 11)

add_executable(dogwater_16 main.c
        alu.c
        main.h
        control_flow.c
        memory.c)
