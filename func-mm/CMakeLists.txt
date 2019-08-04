cmake_minimum_required(VERSION 3.12)
project(func_mm C)

set(CMAKE_C_STANDARD 11)

add_executable(func_mm
        montyfunc.c monty.h)
