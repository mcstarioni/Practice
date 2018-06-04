cmake_minimum_required(VERSION 3.9)
project(Practice C)

set(CMAKE_C_STANDARD 11)

add_executable(Practice main.c FileBuffer.h Queue.h Queue.c)