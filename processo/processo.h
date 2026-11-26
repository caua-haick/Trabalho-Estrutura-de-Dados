//
// Created by caua2 on 26/11/2026.
//

#ifndef TRABALHO_AJALMAR_PROCESSO_H
#define TRABALHO_AJALMAR_PROCESSO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define TAM_NOME 20
#define TAM_DESC 256
#define TAM_LINHA 300

typedef struct processo {
    char nome[TAM_NOME];
    char descricao[TAM_DESC];
    struct processo *prox;
} processo;

void enfileirar_processo(processo **inicio, const char *nome, const char *descricao);
int identificar_processo(const char *linha, char *nome_processo);
int ler_script(const char *caminho, processo **fila);

processo *proximo_processo(processo **fila);
void enfileirar_no(processo **inicio, processo *no);
int identificar_tabela(const processo *p, char *tabela);
void distribuir_processos(processo **geral, processo **fila_pessoa,
                          processo **fila_pet, processo **fila_tipo_pet);

#endif //TRABALHO_AJALMAR_PROCESSO_H
