//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PESSOA_H
#define TRABALHO_AJALMAR_PESSOA_H

typedef struct pessoa {
    int codigo;
    char nome[50];
    char fone[20];       // Alterado para vetor de char para aceitar caracteres como parênteses/traços se necessário
    char endereco[100];
    char nascimento[11];
} pessoa;

typedef struct lista_pessoa {
    pessoa dados;
    struct lista_pessoa *prox; // Corrigido para struct lista_pessoa
    struct lista_pessoa *ant;  // Corrigido para struct lista_pessoa
} lista_pessoa;

// Alterado para receber lista_pessoa ** e um ponteiro para pessoa *np
int inserir_pessoa(lista_pessoa **p, pessoa *np);

// Alterado para receber lista_pessoa **
int remover_pessoa(lista_pessoa **p);

#endif //TRABALHO_AJALMAR_PESSOA_Hw