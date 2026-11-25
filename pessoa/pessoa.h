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
} pessoa;
typedef struct lista_pessoa{
    pessoa dados;
    struct pessoa *prox;
    struct pessoa *ant;
}lista_pessoa;

int inserir_pessoa(pessoa **p);

int remover_pessoa(pessoa **p);

#endif //TRABALHO_AJALMAR_PESSOA_H
