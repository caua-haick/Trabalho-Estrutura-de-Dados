//
// Created by caua2 on 24/11/2026.
//

#include "pessoa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int inserir_pessoa(lista_pessoa **p, pessoa *np) {
    // Inserir pessoa na lista encadeada
    lista_pessoa *novo = (lista_pessoa *) malloc(sizeof(lista_pessoa));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }

    // Atribuindo campos numéricos e copiando strings com segurança
    novo->dados.codigo = np->codigo;
    strcpy(novo->dados.nome, np->nome);
    strcpy(novo->dados.fone, np->fone);
    strcpy(novo->dados.endereco, np->endereco);
    strcpy(novo->dados.nascimento, np->nascimento);

    novo->ant = NULL;
    novo->prox = NULL;

    lista_pessoa *aux = *p;
    if (!aux) {
        *p = novo;
        return 1;
    }
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
    novo->ant = aux;
    return 1;
}

int remover_pessoa(lista_pessoa **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    lista_pessoa *aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}