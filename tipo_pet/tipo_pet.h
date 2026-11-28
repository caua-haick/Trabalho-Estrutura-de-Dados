//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_TIPO_PET_H
#define TRABALHO_AJALMAR_TIPO_PET_H

#include "../processo/processo.h"

#define ARQ_TIPO_PET "tipo_pet.bin"

typedef struct tipo_pet {
    int codigo;
    char nome[50];
} tipo_pet;
typedef struct lista_tipo_pet {
    tipo_pet dados;
    struct lista_tipo_pet *prox;
    struct lista_tipo_pet *ant;
} lista_tipo_pet;

typedef struct arvore_tipo_pet {
    tipo_pet dados;
    struct arvore_tipo_pet *esq;
    struct arvore_tipo_pet *dir;
} arvore_tipo_pet;


int inserir_tipo_pet(lista_tipo_pet **p, tipo_pet *np);


lista_tipo_pet *buscar_tipo_pet(lista_tipo_pet **p, int codigo);

int atualizar_tipo_pet(lista_tipo_pet **p, atualizacao *a);

int mostrar_tipo_pet(lista_tipo_pet **p, consulta *c);

int remover_tipo_pet(lista_tipo_pet **p, const char *campo, int valor);

int carregar_tipo_pet(lista_tipo_pet **p, const char *arquivo);

int salvar_tipo_pet(lista_tipo_pet **p, const char *arquivo);

void liberar_tipo_pet(lista_tipo_pet **p);

int interpretar_insert_tipo_pet(const char *descricao, tipo_pet *t);

int interpretar_select_tipo_pet(const char *descricao, consulta *c);

int interpretar_update_tipo_pet(const char *descricao, atualizacao *a);

int Realizar_Processo_Tipo_Pet(lista_tipo_pet **p, processo **fila_tipo_pet);

#endif //TRABALHO_AJALMAR_TIPO_PET_H
