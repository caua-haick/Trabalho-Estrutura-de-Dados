//
// Created by caua2 on 24/11/2026.
//

#include "tipo_pet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int inserir_tipo_pet(lista_tipo_pet **p, tipo_pet *np) {
    lista_tipo_pet *novo = (lista_tipo_pet *) malloc(sizeof(lista_tipo_pet));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }

    // Copiando os dados corretamente
    novo->dados.codigo = np->codigo;
    strcpy(novo->dados.nome, np->nome);

    novo->ant = NULL;
    novo->prox = NULL;

    lista_tipo_pet *aux = *p;
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

int remover_tipo_pet(lista_tipo_pet **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    lista_tipo_pet *aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}