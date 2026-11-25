//
// Created by caua2 on 24/11/2026.
//

#include "pet.h"
#include <stdlib.h>
#include <stdio.h>

int inserir_pet(pet **p) {
    //inserir pet e escrever no arquivo
    pet *novo = (pet *) malloc(sizeof(pet));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }
    novo->codigo = c;
    novo->nome = n;
    novo->codigo_pessoa = cp;
    novo->codigo_tipo_pet = ctp;
    novo->ant = NULL;
    novo->prox = NULL;
    pet* aux = *p;
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

int remover_pet(pet **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    struct pet* aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}
