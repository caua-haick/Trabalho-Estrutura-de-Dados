//
// Created by caua2 on 24/11/2026.
//

#include "pet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int inserir_pet(lista_pet **p, pet *np) {
    //inserir pet e escrever no arquivo
    lista_pet *novo = (lista_pet *) malloc(sizeof(lista_pet));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }
    novo->dados.codigo = np->codigo;
    strcpy(novo->dados.nome, np->nome);
    novo->dados.codigo_pessoa = np->codigo_pessoa;
    novo->dados.codigo_tipo_pet = np->codigo_tipo_pet;
    novo->ant = NULL;
    novo->prox = NULL;
    lista_pet* aux = *p;
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

int remover_pet(lista_pet **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    lista_pet* aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}

int interpretar_insert_pet(const char *descricao, pet *p) {
    int fim = 0;

    sscanf(descricao,
           "INSERT INTO pet (codigo, codigo_pessoa, nome, codigo_tipo_pet) "
           "VALUES (%d, %d, '%49[^']', %d);%n",
           &p->codigo, &p->codigo_pessoa, p->nome, &p->codigo_tipo_pet, &fim);

    return fim > 0;
}