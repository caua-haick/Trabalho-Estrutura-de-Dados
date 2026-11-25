cmake_minimum_required(VERSION 4.4)
project(trabalho_ajalmar C)

set(CMAKE_C_STANDARD 11)

add_executable(trabalho_ajalmar main.c
        pet/pet.c
        pet/pet.h
        pessoa/pessoa.c
        pessoa/pessoa.h
        tipo_pet/tipo_pet.c
        tipo_pet/tipo_pet.h)
