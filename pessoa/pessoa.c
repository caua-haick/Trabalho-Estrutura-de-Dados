//
// Created by caua2 on 24/11/2026.
//

#include "pessoa.h"
#include <stdlib.h>
#include <stdio.h>

int inserir_pessoa(pessoa **p) {
    //inserir pet e escrever no arquivo
    pessoa *novo = (pessoa *) malloc(sizeof(pessoa));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }
    novo->codigo = c;
    novo->nome = n;
    novo->fone = f;
    novo->endereco = e;
    novo->nascimento = nas;
    novo->ant = NULL;
    novo->prox = NULL;
    pessoa* aux = *p;
    if (!aux) {
        *p = novo;
        return 1;
    }
    while (aux && aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = novo;
    novo->ant = aux;
    return 1;
};

int remover_pessoa(pessoa **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    struct pessoa* aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}
