cmake_minimum_required(VERSION 4.2)
project(TesteTrabalho C)

set(CMAKE_C_STANDARD 11)

add_executable(TesteTrabalho main.c
        pessoa/pessoa.c
        pessoa/pessoa.h
        pet/pet.c
        pet/pet.h
        processo/processo.c
        processo/processo.h
        tipo_pet/tipo_pet.c
        tipo_pet/tipo_pet.h)
