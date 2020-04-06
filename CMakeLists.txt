cmake_minimum_required(VERSION 3.13)
project(ProjetC C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetC main.c cmake-build-debug/fir.c cmake-build-debug/fir.h)