cmake_minimum_required(VERSION 3.8)
project(brainfuck)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES main.c brainfuck/brainfuck.c)
add_executable(brainfuck ${SOURCE_FILES})