//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PET_H
#define TRABALHO_AJALMAR_PET_H

#include "../processo/processo.h"

#define ARQ_PET "pet.bin"

typedef struct pet {
    int codigo;
    int codigo_pessoa;
    char nome[50];
    int codigo_tipo_pet;
} pet;
typedef struct lista_pet{
    pet dados;
    struct lista_pet *prox;
    struct lista_pet *ant;
}lista_pet;

typedef struct arvore_pet {
    pet dados;
    struct arvore_pet *esq;
    struct arvore_pet *dir;
} arvore_pet;

int inserir_pet(lista_pet **p,pet *np);

lista_pet *buscar_pet(lista_pet **p, int codigo);

int atualizar_pet(lista_pet **p, atualizacao *a);

int mostrar_pet(lista_pet **p, consulta *c);

int remover_pet(lista_pet **p, const char *campo, int valor);

int carregar_pet(lista_pet **p, const char *arquivo);

int salvar_pet(lista_pet **p, const char *arquivo);

void liberar_pet(lista_pet **p);

int interpretar_insert_pet(const char *descricao, pet *p);

int interpretar_select_pet(const char *descricao, consulta *c);

int interpretar_update_pet(const char *descricao, atualizacao *a);

int Realizar_Processo_Pet(lista_pet **p, processo **fila_pet);

int inserir_arvore_pet(arvore_pet **raiz, const pet *dados);

void mostrar_em_ordem_pet(arvore_pet **raiz);

void liberar_arvore_pet(arvore_pet **raiz);

#endif //TRABALHO_AJALMAR_PET_H
