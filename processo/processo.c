//
// Created by Kaio Marcos on 26/11/2026.
//
#include "processo.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inserir_processo(processo **inicio, char *nome, char *descricao) {
    processo *novo = (processo *) malloc(sizeof(processo));
    if (!novo) {
        printf("Erro em alocar memoria.\n");
        return;
    }

    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    strncpy(novo->descricao, descricao, TAM_DESC - 1);
    novo->descricao[TAM_DESC - 1] = '\0';
    novo->prox = NULL;

    if (!*inicio) {
        *inicio = novo;
    } else {
        processo *aux = *inicio;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

int identificar_comando(const char *linha, char *operacao, char *tabela) {
    if (sscanf(linha, "INSERT INTO %19[a-z_]", tabela) == 1) {
        strcpy(operacao, "inserir");
    } else if (sscanf(linha, "SELECT * FROM %19[a-z_]", tabela) == 1) {
        strcpy(operacao, "mostrar");
    } else if (sscanf(linha, "UPDATE %19[a-z_]", tabela) == 1) {
        strcpy(operacao, "atualizar");
    } else if (sscanf(linha, "DELETE FROM %19[a-z_]", tabela) == 1) {
        strcpy(operacao, "remover");
    } else {
        return 0;
    }
    return 1;
}

int ler_script(char *caminho, processo **fila) {
    FILE *arq = fopen(caminho, "r");
    char linha[TAM_LINHA];
    char nome_processo[TAM_NOME];
    char tabela[TAM_NOME];
    int numero_linha = 0;

    if (!arq) {
        printf("Erro: nao foi possivel abrir arquivo! %s\n", caminho);
        return 0;
    }

    while (fgets(linha, TAM_LINHA, arq) != NULL) {
        numero_linha++;
        linha[strcspn(linha, "\r\n")] = '\0';

        if (linha[0] == '\0') {
            continue;
        }

        if (identificar_comando(linha, nome_processo, tabela)) {
            inserir_processo(fila, nome_processo, linha);
        } else {
            printf("Aviso: comando nao reconhecido na linha %d\n", numero_linha);
        }
    }

    fclose(arq);
    return 1;
}

processo *proximo_processo(processo **fila) {
    processo *removido = *fila;

    if (removido == NULL) {
        return NULL;
    }

    *fila = removido->prox;
    removido->prox = NULL;
    return removido;
}

void enfileirar_no(processo **inicio, processo *no) {
    no->prox = NULL;

    if (*inicio == NULL) {
        *inicio = no;
    } else {
        processo *aux = *inicio;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = no;
    }
}

void distribuir_processos(processo **geral, processo **fila_pessoa,
                          processo **fila_pet, processo **fila_tipo_pet) {
    processo *aux;
    char tabela[TAM_NOME];
    char operacao[TAM_NOME];

    while ((aux = proximo_processo(geral)) != NULL) {
        if (identificar_comando(aux->descricao, operacao, tabela) == 0) {
            printf("Aviso: tabela nao identificada, processo descartado: %s\n", aux->descricao);
            free(aux);
        } else if (strcmp(tabela, "pessoa") == 0) {
            enfileirar_no(fila_pessoa, aux);
        } else if (strcmp(tabela, "pet") == 0) {
            enfileirar_no(fila_pet, aux);
        } else if (strcmp(tabela, "tipo_pet") == 0) {
            enfileirar_no(fila_tipo_pet, aux);
        } else {
            printf("Aviso: tabela %s desconhecida, processo descartado: %s\n", tabela, aux->descricao);
            free(aux);
        }
    }
}

int interpretar_select(const char *descricao, consulta *c) {
    int fim;

    c->tem_filtro = 0;
    c->tem_ordem = 0;

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_] %19[a-z_] = %d ORDER BY %19[a-z_];%n",
           c->campo_filtro, &c->valor_filtro, c->campo_ordem, &fim);
    if (fim > 0) {
        c->tem_filtro = 1;
        c->tem_ordem = 1;
        return 1;
    }

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_] %19[a-z_] = %d;%n",
           c->campo_filtro, &c->valor_filtro, &fim);
    if (fim > 0) {
        c->tem_filtro = 1;
        return 1;
    }

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_] ORDER BY %19[a-z_];%n",
           c->campo_ordem, &fim);
    if (fim > 0) {
        c->tem_ordem = 1;
        return 1;
    }

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_];%n", &fim);
    if (fim > 0) {
        return 1;
    }

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_] WHERE %19[a-z_] = %d ORDER BY %19[a-z_];%n",
           c->campo_filtro, &c->valor_filtro, c->campo_ordem, &fim);
    if (fim > 0) {
        c->tem_filtro = 1;
        c->tem_ordem = 1;
        return 1;
    }

    fim = 0;
    sscanf(descricao, "SELECT * FROM %*[a-z_] WHERE %19[a-z_] = %d;%n",
           c->campo_filtro, &c->valor_filtro, &fim);
    if (fim > 0) {
        c->tem_filtro = 1;
        return 1;
    }

    return 0;
}

int interpretar_delete(const char *descricao, consulta *c) {
    int fim = 0;

    c->tem_filtro = 0;
    c->tem_ordem = 0;

    sscanf(descricao, "DELETE FROM %*[a-z_] WHERE %19[a-z_] = %d;%n",
           c->campo_filtro, &c->valor_filtro, &fim);
    if (fim > 0) {
        c->tem_filtro = 1;
        return 1;
    }

    return 0;
}

int campo_valido(const char *campo, const char *validos[], int qtd) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp(campo, validos[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int comparar_inteiros(int a, int b) {
    if (a < b) {
        return -1;
    }
    if (a > b) {
        return 1;
    }
    return 0;
}
