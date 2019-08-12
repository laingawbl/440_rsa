cmake_minimum_required(VERSION 3.12)
project(opt_mm C)

set(CMAKE_C_STANDARD 11)

add_executable(opt_mm
        monty.c monty.h main.c)
