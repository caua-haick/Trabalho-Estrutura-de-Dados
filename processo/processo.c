//
// Created by Kaio Marcos on 26/11/2026.
//
#include "../processo.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void enfileirar_processo(processo **inicio, char *nome, char *descricao) {
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

    if (*inicio == NULL) {
        *inicio = novo;
    } else {
        processo *aux = *inicio;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
}

int identificar_processo(char *linha, char *nome_processo) {
    char comando[16];
    int i;

    if (sscanf(linha, "%15s", comando) != 1) {
        return 0;
    }

    for (i = 0; comando[i] != '\0'; i++) {
        comando[i] = toupper((unsigned char) comando[i]);
    }

    if (strcmp(comando, "INSERT") == 0) {
        strcpy(nome_processo, "inserir");
    } else if (strcmp(comando, "SELECT") == 0) {
        strcpy(nome_processo, "mostrar");
    } else if (strcmp(comando, "UPDATE") == 0) {
        strcpy(nome_processo, "atualizar");
    } else if (strcmp(comando, "DELETE") == 0) {
        strcpy(nome_processo, "remover");
    } else {
        return 0;
    }
    return 1;
}

int ler_script(char *caminho, processo **fila) {
    FILE *arq = fopen(caminho, "r");
    char linha[TAM_LINHA];
    char nome_processo[TAM_NOME];
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

        if (identificar_processo(linha, nome_processo)) {
            enfileirar_processo(fila, nome_processo, linha);
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

int identificar_tabela(const processo *p, char *tabela) {
    char copia[TAM_DESC];
    const char *palavra_chave;
    char *pos;
    int i, j;

    if (strcmp(p->nome, "inserir") == 0) {
        palavra_chave = "INTO";
    } else if (strcmp(p->nome, "atualizar") == 0) {
        palavra_chave = "UPDATE";
    } else {
        palavra_chave = "FROM";
    }

    for (i = 0; p->descricao[i] != '\0'; i++) {
        copia[i] = toupper((unsigned char) p->descricao[i]);
    }
    copia[i] = '\0';

    pos = strstr(copia, palavra_chave);
    if (pos == NULL) {
        return 0;
    }
    pos += strlen(palavra_chave);

    while (isspace((unsigned char) *pos)) {
        pos++;
    }

    j = 0;
    while ((isalnum((unsigned char) *pos) || *pos == '_') && j < TAM_NOME - 1) {
        tabela[j] = *pos;
        j++;
        pos++;
    }
    tabela[j] = '\0';

    if (j == 0) {
        return 0;
    }
    return 1;
}

void distribuir_processos(processo **geral, processo **fila_pessoa,
                          processo **fila_pet, processo **fila_tipo_pet) {
    processo *atual;
    char tabela[TAM_NOME];

    while ((atual = proximo_processo(geral)) != NULL) {
        if (identificar_tabela(atual, tabela) == 0) {
            printf("Aviso: tabela nao identificada, processo descartado: %s\n", atual->descricao);
            free(atual);
        } else if (strcmp(tabela, "PESSOA") == 0) {
            enfileirar_no(fila_pessoa, atual);
        } else if (strcmp(tabela, "PET") == 0) {
            enfileirar_no(fila_pet, atual);
        } else if (strcmp(tabela, "TIPO_PET") == 0) {
            enfileirar_no(fila_tipo_pet, atual);
        } else {
            printf("Aviso: tabela %s desconhecida, processo descartado: %s\n", tabela, atual->descricao);
            free(atual);
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
