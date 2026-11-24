cmake_minimum_required(VERSION 4.4)
project(trabalho_ajalmar C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalho_ajalmar main.c
        pet.c
        pet.h
        pessoa.c
        pessoa.h
        tipo_pet.c
        tipo_pet.h)
