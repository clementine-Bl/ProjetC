cmake_minimum_required(VERSION 3.13)
project(ProjetC C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetC main.c fir.c fir.h affichage.c affichage.h define.h fichiers.c fichiers.h integration.c integration.h iir.c iir.h mesure.c mesure.h)