//
// Created by caua2 on 24/11/2026.
//

#include "tipo_pet.h"

#include <stdlib.h>
#include <stdio.h>
int inserir_tipo_pet(tipo_pet **p) {
    //inserir pet e escrever no arquivo
    tipo_pet *novo = (tipo_pet *) malloc(sizeof(tipo_pet));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }
    //novo->codigo = ;
    //novo->nome = n;
    novo->ant = NULL;
    novo->prox = NULL;
    tipo_pet* aux = *p;
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

int remover_tipo_pet(tipo_pet **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    struct tipo_pet* aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}
