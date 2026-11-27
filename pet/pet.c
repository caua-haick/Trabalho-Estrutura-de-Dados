//
// Created by caua2 on 24/11/2026.
//

#include "../pet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int inserir_pet(lista_pet **p, pet *np) {
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
}

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

int interpretar_update_pet(const char *descricao, atualizacao *a) {
    int fim;

    a->valor_int = 0;
    a->valor_texto[0] = '\0';

    fim = 0;
    sscanf(descricao, "UPDATE pet SET nome = '%49[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "nome");
        return 1;
    }

    fim = 0;
    sscanf(descricao, "UPDATE pet SET codigo_pessoa = %d WHERE codigo = %d;%n",
           &a->valor_int, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "codigo_pessoa");
        return 1;
    }

    fim = 0;
    sscanf(descricao, "UPDATE pet SET codigo_tipo_pet = %d WHERE codigo = %d;%n",
           &a->valor_int, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "codigo_tipo_pet");
        return 1;
    }

    return 0;
}

int interpretar_select_pet(const char *descricao, consulta *c) {
    const char *filtros[] = {"codigo", "codigo_pessoa", "codigo_tipo_pet"};
    const char *ordens[] = {"codigo", "codigo_pessoa", "nome", "codigo_tipo_pet"};

    if (!interpretar_select(descricao, c)) {
        return 0;
    }
    if (c->tem_filtro && !campo_valido(c->campo_filtro, filtros, 3)) {
        return 0;
    }
    if (c->tem_ordem && !campo_valido(c->campo_ordem, ordens, 4)) {
        return 0;
    }
    return 1;
}

lista_pet *buscar_pet(lista_pet **p, int codigo) {
    lista_pet *aux = *p;

    while (aux != NULL && aux->dados.codigo != codigo) {
        aux = aux->prox;
    }
    return aux;
}

int atualizar_pet(lista_pet **p, atualizacao *a) {
    lista_pet *no = buscar_pet(p, a->codigo);

    if (!no) {
        return 0;
    }

    if (strcmp(a->campo, "nome") == 0) {
        strcpy(no->dados.nome, a->valor_texto);
    } else if (strcmp(a->campo, "codigo_pessoa") == 0) {
        no->dados.codigo_pessoa = a->valor_int;
    } else if (strcmp(a->campo, "codigo_tipo_pet") == 0) {
        no->dados.codigo_tipo_pet = a->valor_int;
    } else {
        return 0;
    }
    return 1;
}

static int valor_do_campo(const pet *x, const char *campo, int *valor) {
    if (strcmp(campo, "codigo") == 0) {
        *valor = x->codigo;
    } else if (strcmp(campo, "codigo_pessoa") == 0) {
        *valor = x->codigo_pessoa;
    } else if (strcmp(campo, "codigo_tipo_pet") == 0) {
        *valor = x->codigo_tipo_pet;
    } else {
        return 0;
    }
    return 1;
}

int RemoverPorParametro_pet(lista_pet **p, const char *campo, int valor) {
    const char *validos[] = {"codigo", "codigo_pessoa", "codigo_tipo_pet"};
    lista_pet *aux = *p;
    lista_pet *prox;
    int atual = 0;
    int removidos = 0;

    if (!campo_valido(campo, validos, 3)) {
        return -1;
    }

    while (aux != NULL) {
        prox = aux->prox;
        valor_do_campo(&aux->dados, campo, &atual);

        if (atual == valor) {
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

static int corresponde_pet(const pet *x, const consulta *c) {
    int valor = 0;

    if (!c->tem_filtro) {
        return 1;
    }
    if (!valor_do_campo(x, c->campo_filtro, &valor)) {
        return 0;
    }
    return valor == c->valor_filtro;
}

static int comparar_pet(const pet *a, const pet *b, const char *campo) {
    if (strcmp(campo, "codigo") == 0) {
        return comparar_inteiros(a->codigo, b->codigo);
    }
    if (strcmp(campo, "codigo_pessoa") == 0) {
        return comparar_inteiros(a->codigo_pessoa, b->codigo_pessoa);
    }
    if (strcmp(campo, "codigo_tipo_pet") == 0) {
        return comparar_inteiros(a->codigo_tipo_pet, b->codigo_tipo_pet);
    }
    if (strcmp(campo, "nome") == 0) {
        return strcmp(a->nome, b->nome);
    }
    return 0;
}

int mostrar_pet(lista_pet **p, consulta *c) {
    lista_pet *aux;
    pet *vet;
    pet tmp;
    int n = 0;
    int i, j;

    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_pet(&aux->dados, c)) {
            n++;
        }
    }

    if (n == 0) {
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    vet = (pet *) malloc(n * sizeof(pet));
    if (!vet) {
        printf("Erro em alocar memoria\n");
        return 0;
    }

    i = 0;
    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_pet(&aux->dados, c)) {
            vet[i] = aux->dados;
            i++;
        }
    }

    if (c->tem_ordem) {
        for (i = 1; i < n; i++) {
            tmp = vet[i];
            j = i - 1;
            while (j >= 0 && comparar_pet(&vet[j], &tmp, c->campo_ordem) > 0) {
                vet[j + 1] = vet[j];
                j--;
            }
            vet[j + 1] = tmp;
        }
    }

    printf("codigo | codigo_pessoa | nome | codigo_tipo_pet\n");
    for (i = 0; i < n; i++) {
        printf("%d | %d | %s | %d\n", vet[i].codigo, vet[i].codigo_pessoa,
               vet[i].nome, vet[i].codigo_tipo_pet);
    }
    printf("(%d registro(s))\n", n);

    free(vet);
    return n;
}

int carregar_pet(lista_pet **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "rb");
    pet temp;

    if (!arq) {
        return 1;
    }

    while (fread(&temp, sizeof(pet), 1, arq) == 1) {
        inserir_pet(p, &temp);
    }

    fclose(arq);
    return 1;
}

int salvar_pet(lista_pet **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "wb");
    lista_pet *aux;

    if (!arq) {
        printf("Erro: nao foi possivel gravar %s\n", arquivo);
        return 0;
    }

    for (aux = *p; aux != NULL; aux = aux->prox) {
        fwrite(&aux->dados, sizeof(pet), 1, arq);
    }

    fclose(arq);
    return 1;
}

void liberar_pet(lista_pet **p) {
    lista_pet *aux;

    while (*p != NULL) {
        aux = *p;
        *p = aux->prox;
        free(aux);
    }
}

int Realizar_Processo_Pet(lista_pet **p, processo **fila_pet) {
    processo *atual;
    pet np;
    consulta c;
    atualizacao a;
    int erros = 0;
    int ok;
    int removidos;

    while ((atual = proximo_processo(fila_pet)) != NULL) {
        ok = 0;

        if (strcmp(atual->nome, "inserir") == 0) {
            if (!interpretar_insert_pet(atual->descricao, &np)) {
                printf("[pet] Sintaxe invalida: %s\n", atual->descricao);
            } else if (buscar_pet(p, np.codigo) != NULL) {
                printf("[pet] Codigo %d ja existe, insercao ignorada.\n", np.codigo);
            } else if (inserir_pet(p, &np)) {
                printf("[pet] Inserido: codigo %d\n", np.codigo);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "mostrar") == 0) {
            if (!interpretar_select_pet(atual->descricao, &c)) {
                printf("[pet] Sintaxe invalida: %s\n", atual->descricao);
            } else {
                printf("[pet] %s\n", atual->descricao);
                mostrar_pet(p, &c);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "atualizar") == 0) {
            if (!interpretar_update_pet(atual->descricao, &a)) {
                printf("[pet] Sintaxe invalida: %s\n", atual->descricao);
            } else if (!atualizar_pet(p, &a)) {
                printf("[pet] Codigo %d nao encontrado, nada atualizado.\n", a.codigo);
            } else {
                printf("[pet] Atualizado: codigo %d, campo %s\n", a.codigo, a.campo);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "remover") == 0) {
            if (!interpretar_delete(atual->descricao, &c)) {
                printf("[pet] Sintaxe invalida: %s\n", atual->descricao);
            } else {
                removidos = RemoverPorParametro_pet(p, c.campo_filtro, c.valor_filtro);
                if (removidos < 0) {
                    printf("[pet] Campo %s nao pode ser usado no WHERE.\n", c.campo_filtro);
                } else {
                    printf("[pet] %d registro(s) removido(s) (%s = %d)\n",
                           removidos, c.campo_filtro, c.valor_filtro);
                    ok = 1;
                }
            }
        } else {
            printf("[pet] Processo desconhecido: %s\n", atual->nome);
        }

        if (!ok) {
            erros++;
        }
        free(atual);
    }

    return erros;
}
