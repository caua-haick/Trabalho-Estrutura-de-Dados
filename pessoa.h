//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PESSOA_H
#define TRABALHO_AJALMAR_PESSOA_H
typedef struct pessoa {
    int codigo;
    char nome[50];
    int fone;
    char endereco[100];
    char nascimento[11];
    struct pessoa *prox;
    struct pessoa *ant;
} pessoa;
#endif //TRABALHO_AJALMAR_PESSOA_H
