//
// Created by Kaio Marcos on 26/11/2026.
//

#ifndef TRABALHO_ESTRUTURA_DE_DADOS_PROCESSO_H
#define TRABALHO_ESTRUTURA_DE_DADOS_PROCESSO_H

#define TAM_NOME 20
#define TAM_DESC 512
#define TAM_LINHA 600
#define TAM_VALOR 100

typedef struct processo {
    char nome[TAM_NOME];
    char descricao[TAM_DESC];
    struct processo *prox;
} processo;

typedef struct consulta {
    int tem_filtro;
    char campo_filtro[TAM_NOME];
    int valor_filtro;
    int tem_ordem;
    char campo_ordem[TAM_NOME];
} consulta;

typedef struct atualizacao {
    int codigo;
    char campo[TAM_NOME];
    int valor_int;
    char valor_texto[TAM_VALOR];
} atualizacao;

void inserir_processo(processo **inicio, char *nome, char *descricao);

int identificar_comando(const char *linha, char *operacao, char *tabela);

int ler_script(char *caminho, processo **fila);


processo *proximo_processo(processo **fila);

void enfileirar_no(processo **inicio, processo *no);

void distribuir_processos(processo **geral, processo **fila_pessoa,
                          processo **fila_pet, processo **fila_tipo_pet);

int interpretar_select(const char *descricao, consulta *c);

int interpretar_delete(const char *descricao, consulta *c);

int campo_valido(const char *campo, const char *validos[], int qtd);

int comparar_inteiros(int a, int b);

#endif //TRABALHO_ESTRUTURA_DE_DADOS_PROCESSO_H
