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


int interpretar_insert_tipo_pet(const char *descricao, tipo_pet *t) {
    int fim = 0;

    sscanf(descricao,
           "INSERT INTO tipo_pet (codigo, nome) VALUES (%d, '%49[^']');%n",
           &t->codigo, t->nome, &fim);

    return fim > 0;
}


int interpretar_update_tipo_pet(const char *descricao, atualizacao *a) {
    int fim = 0;

    a->valor_int = 0;
    a->valor_texto[0] = '\0';

    sscanf(descricao, "UPDATE tipo_pet SET nome = '%49[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "nome");
        return 1;
    }

    return 0;
}

int interpretar_select_tipo_pet(const char *descricao, consulta *c) {
    const char *filtros[] = {"codigo"};
    const char *ordens[] = {"codigo", "nome"};

    if (!interpretar_select(descricao, c)) {
        return 0;
    }
    if (c->tem_filtro && !campo_valido(c->campo_filtro, filtros, 1)) {
        return 0;
    }
    if (c->tem_ordem && !campo_valido(c->campo_ordem, ordens, 2)) {
        return 0;
    }
    return 1;
}

lista_tipo_pet *buscar_tipo_pet(lista_tipo_pet **p, int codigo) {
    lista_tipo_pet *aux = *p;

    while (aux != NULL && aux->dados.codigo != codigo) {
        aux = aux->prox;
    }
    return aux;
}

int atualizar_tipo_pet(lista_tipo_pet **p, atualizacao *a) {
    lista_tipo_pet *no = buscar_tipo_pet(p, a->codigo);

    if (!no) {
        return 0;
    }

    if (strcmp(a->campo, "nome") == 0) {
        strcpy(no->dados.nome, a->valor_texto);
    } else {
        return 0;
    }
    return 1;
}

int remover_tipo_pet(lista_tipo_pet **p, const char *campo, int valor) {
    lista_tipo_pet *aux = *p;
    lista_tipo_pet *prox;
    int removidos = 0;

    if (strcmp(campo, "codigo") != 0) {
        return -1;
    }

    while (aux != NULL) {
        prox = aux->prox;

        if (aux->dados.codigo == valor) {
            if (aux->ant != NULL) {
                aux->ant->prox = aux->prox;
            } else {
                *p = aux->prox;
            }
            if (aux->prox != NULL) {
                aux->prox->ant = aux->ant;
            }
            free(aux);
            removidos++;
        }

        aux = prox;
    }
    return removidos;
}

static int corresponde_tipo_pet(const tipo_pet *x, const consulta *c) {
    if (!c->tem_filtro) {
        return 1;
    }
    return x->codigo == c->valor_filtro;
}

static int comparar_tipo_pet(const tipo_pet *a, const tipo_pet *b, const char *campo) {
    if (strcmp(campo, "codigo") == 0) {
        return comparar_inteiros(a->codigo, b->codigo);
    }
    if (strcmp(campo, "nome") == 0) {
        return strcmp(a->nome, b->nome);
    }
    return 0;
}

int mostrar_tipo_pet(lista_tipo_pet **p, consulta *c) {
    lista_tipo_pet *aux;
    tipo_pet *vet;
    tipo_pet tmp;
    int n = 0;
    int i, j;

    if (c->tem_ordem && strcmp(c->campo_ordem, "nome") == 0) {
        return mostrar_tipo_pet_por_nome(p, c);
    }

    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_tipo_pet(&aux->dados, c)) {
            n++;
        }
    }

    if (n == 0) {
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    vet = (tipo_pet *) malloc(n * sizeof(tipo_pet));
    if (!vet) {
        printf("Erro em alocar memoria\n");
        return 0;
    }

    i = 0;
    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_tipo_pet(&aux->dados, c)) {
            vet[i] = aux->dados;
            i++;
        }
    }

    if (c->tem_ordem) {
        for (i = 1; i < n; i++) {
            tmp = vet[i];
            j = i - 1;
            while (j >= 0 && comparar_tipo_pet(&vet[j], &tmp, c->campo_ordem) > 0) {
                vet[j + 1] = vet[j];
                j--;
            }
            vet[j + 1] = tmp;
        }
    }

    printf("codigo | nome\n");
    for (i = 0; i < n; i++) {
        printf("%d | %s\n", vet[i].codigo, vet[i].nome);
    }
    printf("(%d registro(s))\n", n);

    free(vet);
    return n;
}

int carregar_tipo_pet(lista_tipo_pet **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "rb");
    tipo_pet temp;

    if (!arq) {
        return 1;
    }

    while (fread(&temp, sizeof(tipo_pet), 1, arq) == 1) {
        inserir_tipo_pet(p, &temp);
    }

    fclose(arq);
    return 1;
}

int salvar_tipo_pet(lista_tipo_pet **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "wb");
    lista_tipo_pet *aux;

    if (!arq) {
        printf("Erro: nao foi possivel gravar %s\n", arquivo);
        return 0;
    }

    for (aux = *p; aux != NULL; aux = aux->prox) {
        fwrite(&aux->dados, sizeof(tipo_pet), 1, arq);
    }

    fclose(arq);
    return 1;
}

void liberar_tipo_pet(lista_tipo_pet **p) {
    lista_tipo_pet *aux;

    while (*p != NULL) {
        aux = *p;
        *p = aux->prox;
        free(aux);
    }
}

int Realizar_Processo_Tipo_Pet(lista_tipo_pet **p, processo **fila_tipo_pet) {
    processo *aux;
    tipo_pet np;
    consulta c;
    atualizacao a;
    int erros = 0;
    int ok;
    int removidos;

    while ((aux = proximo_processo(fila_tipo_pet)) != NULL) {
        ok = 0;

        if (strcmp(aux->nome, "inserir") == 0) {
            if (!interpretar_insert_tipo_pet(aux->descricao, &np)) {
                printf("[tipo_pet] Sintaxe invalida: %s\n", aux->descricao);
            } else if (buscar_tipo_pet(p, np.codigo) != NULL) {
                printf("[tipo_pet] Codigo %d ja existe, insercao ignorada.\n", np.codigo);
            } else if (inserir_tipo_pet(p, &np)) {
                printf("[tipo_pet] Inserido: codigo %d\n", np.codigo);
                ok = 1;
            }
        } else if (strcmp(aux->nome, "mostrar") == 0) {
            if (!interpretar_select_tipo_pet(aux->descricao, &c)) {
                printf("[tipo_pet] Sintaxe invalida: %s\n", aux->descricao);
            } else {
                printf("[tipo_pet] %s\n", aux->descricao);
                mostrar_tipo_pet(p, &c);
                ok = 1;
            }
        } else if (strcmp(aux->nome, "atualizar") == 0) {
            if (!interpretar_update_tipo_pet(aux->descricao, &a)) {
                printf("[tipo_pet] Sintaxe invalida: %s\n", aux->descricao);
            } else if (!atualizar_tipo_pet(p, &a)) {
                printf("[tipo_pet] Codigo %d nao encontrado, nada atualizado.\n", a.codigo);
            } else {
                printf("[tipo_pet] Atualizado: codigo %d, campo %s\n", a.codigo, a.campo);
                ok = 1;
            }
        } else if (strcmp(aux->nome, "remover") == 0) {
            if (!interpretar_delete(aux->descricao, &c)) {
                printf("[tipo_pet] Sintaxe invalida: %s\n", aux->descricao);
            } else {
                removidos = remover_tipo_pet(p, c.campo_filtro, c.valor_filtro);
                if (removidos < 0) {
                    printf("[tipo_pet] Campo %s nao pode ser usado no WHERE.\n", c.campo_filtro);
                } else {
                    printf("[tipo_pet] %d registro(s) removido(s) (%s = %d)\n",
                           removidos, c.campo_filtro, c.valor_filtro);
                    ok = 1;
                }
            }
        } else {
            printf("[tipo_pet] Processo desconhecido: %s\n", aux->nome);
        }

        if (!ok) {
            erros++;
        }
        free(aux);
    }

    return erros;
}

int inserir_arvore_tipo_pet(arvore_tipo_pet **raiz, const tipo_pet *dados) {
    if (*raiz == NULL) {
        *raiz = (arvore_tipo_pet *) malloc(sizeof(arvore_tipo_pet));
        if (*raiz == NULL) {
            printf("Erro em alocar memoria\n");
            return 0;
        }
        (*raiz)->dados = *dados;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
        return 1;
    }

    if (strcmp(dados->nome, (*raiz)->dados.nome) < 0) {
        return inserir_arvore_tipo_pet(&(*raiz)->esq, dados);
    }
    return inserir_arvore_tipo_pet(&(*raiz)->dir, dados);
}

void mostrar_em_ordem_tipo_pet(arvore_tipo_pet **raiz) {
    if (*raiz == NULL) {
        return;
    }

    mostrar_em_ordem_tipo_pet(&(*raiz)->esq);
    printf("%d | %s\n", (*raiz)->dados.codigo, (*raiz)->dados.nome);
    mostrar_em_ordem_tipo_pet(&(*raiz)->dir);
}

void liberar_arvore_tipo_pet(arvore_tipo_pet **raiz) {
    if (*raiz == NULL) {
        return;
    }

    liberar_arvore_tipo_pet(&(*raiz)->esq);
    liberar_arvore_tipo_pet(&(*raiz)->dir);
    free(*raiz);
    *raiz = NULL;
}

int mostrar_tipo_pet_por_nome(lista_tipo_pet **p, consulta *c) {
    arvore_tipo_pet *raiz = NULL;
    lista_tipo_pet *aux;
    int n = 0;

    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_tipo_pet(&aux->dados, c)) {
            if (inserir_arvore_tipo_pet(&raiz, &aux->dados)) {
                n++;
            }
        }
    }

    if (n == 0) {
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    printf("codigo | nome\n");
    mostrar_em_ordem_tipo_pet(&raiz);
    printf("(%d registro(s))\n", n);

    liberar_arvore_tipo_pet(&raiz);
    return n;
}