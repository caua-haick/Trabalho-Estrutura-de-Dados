//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PESSOA_H
#define TRABALHO_AJALMAR_PESSOA_H

#include "../processo/processo.h"

#define ARQ_PESSOA "pessoa.bin"

typedef struct pessoa {
    int codigo;
    char nome[50];
    char fone[20];
    char endereco[100];
    char nascimento[11];
} pessoa;
typedef struct lista_pessoa {
    pessoa dados;
    struct lista_pessoa *prox;
    struct lista_pessoa *ant;
} lista_pessoa;


int inserir_pessoa(lista_pessoa **p, pessoa *np);

int remover_pessoa(lista_pessoa **p);

lista_pessoa *buscar_pessoa(lista_pessoa **p, int codigo);

int atualizar_pessoa(lista_pessoa **p, atualizacao *a);

int mostrar_pessoa(lista_pessoa **p, consulta *c);

int RemoverPorParametro_pessoa(lista_pessoa **p, const char *campo, int valor);

int carregar_pessoa(lista_pessoa **p, const char *arquivo);

int salvar_pessoa(lista_pessoa **p, const char *arquivo);

void liberar_pessoa(lista_pessoa **p);

int interpretar_insert_pessoa(const char *descricao, pessoa *p);

int interpretar_select_pessoa(const char *descricao, consulta *c);

int interpretar_update_pessoa(const char *descricao, atualizacao *a);

int Realizar_Processo_Pessoa(lista_pessoa **p, processo **fila_pessoa);

#endif //TRABALHO_AJALMAR_PESSOA_H
