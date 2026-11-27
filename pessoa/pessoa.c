//
// Created by caua2 on 24/11/2026.
//

#include "../pessoa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int inserir_pessoa(lista_pessoa **p, pessoa *np) {
    lista_pessoa *novo = (lista_pessoa *) malloc(sizeof(lista_pessoa));
    if (!novo) {
        printf("Erro em alocar memoria");
        return 0;
    }

    novo->dados.codigo = np->codigo;
    strcpy(novo->dados.nome, np->nome);
    strcpy(novo->dados.fone, np->fone);
    strcpy(novo->dados.endereco, np->endereco);
    strcpy(novo->dados.nascimento, np->nascimento);

    novo->ant = NULL;
    novo->prox = NULL;

    lista_pessoa *aux = *p;
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

int remover_pessoa(lista_pessoa **p) {
    if (!*p) {
        printf("Fila vazia!");
        return 0;
    }
    lista_pessoa *aux = *p;
    *p = aux->prox;
    if (*p) {
        (*p)->ant = NULL;
    }
    free(aux);
    return 1;
}

int interpretar_insert_pessoa(const char *descricao, pessoa *p) {
    int fim;

    p->fone[0] = '\0';
    p->endereco[0] = '\0';

    fim = 0;
    sscanf(descricao,
           "INSERT INTO pessoa (codigo, nome, nascimento) "
           "VALUES (%d, '%49[^']', '%10[^']');%n",
           &p->codigo, p->nome, p->nascimento, &fim);
    if (fim > 0) {
        return 1;
    }

    fim = 0;
    sscanf(descricao,
           "INSERT INTO pessoa (codigo, nome, fone, nascimento) "
           "VALUES (%d, '%49[^']', '%19[^']', '%10[^']');%n",
           &p->codigo, p->nome, p->fone, p->nascimento, &fim);
    if (fim > 0) {
        return 1;
    }

    fim = 0;
    sscanf(descricao,
           "INSERT INTO pessoa (codigo, nome, endereco, nascimento) "
           "VALUES (%d, '%49[^']', '%99[^']', '%10[^']');%n",
           &p->codigo, p->nome, p->endereco, p->nascimento, &fim);
    if (fim > 0) {
        return 1;
    }

    fim = 0;
    sscanf(descricao,
           "INSERT INTO pessoa (codigo, nome, fone, endereco, nascimento) "
           "VALUES (%d, '%49[^']', '%19[^']', '%99[^']', '%10[^']');%n",
           &p->codigo, p->nome, p->fone, p->endereco, p->nascimento, &fim);
    if (fim > 0) {
        return 1;
    }

    return 0;
}


int interpretar_update_pessoa(const char *descricao, atualizacao *a) {
    int fim;

    a->valor_int = 0;
    a->valor_texto[0] = '\0';

    fim = 0;
    sscanf(descricao, "UPDATE pessoa SET nome = '%49[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "nome");
        return 1;
    }

    fim = 0;
    sscanf(descricao, "UPDATE pessoa SET fone = '%19[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "fone");
        return 1;
    }

    fim = 0;
    sscanf(descricao, "UPDATE pessoa SET endereco = '%99[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "endereco");
        return 1;
    }

    fim = 0;
    sscanf(descricao, "UPDATE pessoa SET nascimento = '%10[^']' WHERE codigo = %d;%n",
           a->valor_texto, &a->codigo, &fim);
    if (fim > 0) {
        strcpy(a->campo, "nascimento");
        return 1;
    }

    return 0;
}

int interpretar_select_pessoa(const char *descricao, consulta *c) {
    const char *filtros[] = {"codigo"};
    const char *ordens[] = {"codigo", "nome", "fone", "endereco", "nascimento"};

    if (!interpretar_select(descricao, c)) {
        return 0;
    }
    if (c->tem_filtro && !campo_valido(c->campo_filtro, filtros, 1)) {
        return 0;
    }
    if (c->tem_ordem && !campo_valido(c->campo_ordem, ordens, 5)) {
        return 0;
    }
    return 1;
}

lista_pessoa *buscar_pessoa(lista_pessoa **p, int codigo) {
    lista_pessoa *aux = *p;

    while (aux != NULL && aux->dados.codigo != codigo) {
        aux = aux->prox;
    }
    return aux;
}

int atualizar_pessoa(lista_pessoa **p, atualizacao *a) {
    lista_pessoa *no = buscar_pessoa(p, a->codigo);

    if (!no) {
        return 0;
    }

    if (strcmp(a->campo, "nome") == 0) {
        strcpy(no->dados.nome, a->valor_texto);
    } else if (strcmp(a->campo, "fone") == 0) {
        strcpy(no->dados.fone, a->valor_texto);
    } else if (strcmp(a->campo, "endereco") == 0) {
        strcpy(no->dados.endereco, a->valor_texto);
    } else if (strcmp(a->campo, "nascimento") == 0) {
        strcpy(no->dados.nascimento, a->valor_texto);
    } else {
        return 0;
    }
    return 1;
}

int RemoverPorParametro_pessoa(lista_pessoa **p, const char *campo, int valor) {
    lista_pessoa *aux = *p;
    lista_pessoa *prox;
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

static int corresponde_pessoa(const pessoa *x, const consulta *c) {
    if (!c->tem_filtro) {
        return 1;
    }
    return x->codigo == c->valor_filtro;
}

static int comparar_pessoa(const pessoa *a, const pessoa *b, const char *campo) {
    int r;

    if (strcmp(campo, "codigo") == 0) {
        return comparar_inteiros(a->codigo, b->codigo);
    }
    if (strcmp(campo, "nome") == 0) {
        return strcmp(a->nome, b->nome);
    }
    if (strcmp(campo, "fone") == 0) {
        return strcmp(a->fone, b->fone);
    }
    if (strcmp(campo, "endereco") == 0) {
        return strcmp(a->endereco, b->endereco);
    }
    if (strcmp(campo, "nascimento") == 0) {
        r = strncmp(a->nascimento + 6, b->nascimento + 6, 4);
        if (r != 0) {
            return r;
        }
        r = strncmp(a->nascimento + 3, b->nascimento + 3, 2);
        if (r != 0) {
            return r;
        }
        return strncmp(a->nascimento, b->nascimento, 2);
    }
    return 0;
}

int mostrar_pessoa(lista_pessoa **p, consulta *c) {
    lista_pessoa *aux;
    pessoa *vet;
    pessoa tmp;
    int n = 0;
    int i, j;

    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_pessoa(&aux->dados, c)) {
            n++;
        }
    }

    if (n == 0) {
        printf("Nenhum registro encontrado.\n");
        return 0;
    }

    vet = (pessoa *) malloc(n * sizeof(pessoa));
    if (!vet) {
        printf("Erro em alocar memoria\n");
        return 0;
    }

    i = 0;
    for (aux = *p; aux != NULL; aux = aux->prox) {
        if (corresponde_pessoa(&aux->dados, c)) {
            vet[i] = aux->dados;
            i++;
        }
    }

    if (c->tem_ordem) {
        for (i = 1; i < n; i++) {
            tmp = vet[i];
            j = i - 1;
            while (j >= 0 && comparar_pessoa(&vet[j], &tmp, c->campo_ordem) > 0) {
                vet[j + 1] = vet[j];
                j--;
            }
            vet[j + 1] = tmp;
        }
    }

    printf("codigo | nome | fone | endereco | nascimento\n");
    for (i = 0; i < n; i++) {
        printf("%d | %s | %s | %s | %s\n", vet[i].codigo, vet[i].nome,
               vet[i].fone, vet[i].endereco, vet[i].nascimento);
    }
    printf("(%d registro(s))\n", n);

    free(vet);
    return n;
}

int carregar_pessoa(lista_pessoa **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "rb");
    pessoa temp;

    if (!arq) {
        return 1;
    }

    while (fread(&temp, sizeof(pessoa), 1, arq) == 1) {
        inserir_pessoa(p, &temp);
    }

    fclose(arq);
    return 1;
}

int salvar_pessoa(lista_pessoa **p, const char *arquivo) {
    FILE *arq = fopen(arquivo, "wb");
    lista_pessoa *aux;

    if (!arq) {
        printf("Erro: nao foi possivel gravar %s\n", arquivo);
        return 0;
    }

    for (aux = *p; aux != NULL; aux = aux->prox) {
        fwrite(&aux->dados, sizeof(pessoa), 1, arq);
    }

    fclose(arq);
    return 1;
}

void liberar_pessoa(lista_pessoa **p) {
    lista_pessoa *aux;

    while (*p != NULL) {
        aux = *p;
        *p = aux->prox;
        free(aux);
    }
}

static int data_valida(const char *d) {
    int i, dia, mes;

    if (strlen(d) != 10) {
        return 0;
    }

    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (d[i] != '/') {
                return 0;
            }
        } else if (!isdigit((unsigned char) d[i])) {
            return 0;
        }
    }

    dia = (d[0] - '0') * 10 + (d[1] - '0');
    mes = (d[3] - '0') * 10 + (d[4] - '0');
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) {
        return 0;
    }
    return 1;
}

int Realizar_Processo_Pessoa(lista_pessoa **p, processo **fila_pessoa) {
    processo *atual;
    pessoa np;
    consulta c;
    atualizacao a;
    int erros = 0;
    int ok;
    int removidos;

    while ((atual = proximo_processo(fila_pessoa)) != NULL) {
        ok = 0;

        if (strcmp(atual->nome, "inserir") == 0) {
            if (!interpretar_insert_pessoa(atual->descricao, &np)) {
                printf("[pessoa] Sintaxe invalida: %s\n", atual->descricao);
            } else if (!data_valida(np.nascimento)) {
                printf("[pessoa] Nascimento invalido (use dd/mm/aaaa): %s\n", np.nascimento);
            } else if (buscar_pessoa(p, np.codigo) != NULL) {
                printf("[pessoa] Codigo %d ja existe, insercao ignorada.\n", np.codigo);
            } else if (inserir_pessoa(p, &np)) {
                printf("[pessoa] Inserida: codigo %d\n", np.codigo);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "mostrar") == 0) {
            if (!interpretar_select_pessoa(atual->descricao, &c)) {
                printf("[pessoa] Sintaxe invalida: %s\n", atual->descricao);
            } else {
                printf("[pessoa] %s\n", atual->descricao);
                mostrar_pessoa(p, &c);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "atualizar") == 0) {
            if (!interpretar_update_pessoa(atual->descricao, &a)) {
                printf("[pessoa] Sintaxe invalida: %s\n", atual->descricao);
            } else if (strcmp(a.campo, "nascimento") == 0 && !data_valida(a.valor_texto)) {
                printf("[pessoa] Nascimento invalido (use dd/mm/aaaa): %s\n", a.valor_texto);
            } else if (!atualizar_pessoa(p, &a)) {
                printf("[pessoa] Codigo %d nao encontrado, nada atualizado.\n", a.codigo);
            } else {
                printf("[pessoa] Atualizada: codigo %d, campo %s\n", a.codigo, a.campo);
                ok = 1;
            }
        } else if (strcmp(atual->nome, "remover") == 0) {
            if (!interpretar_delete(atual->descricao, &c)) {
                printf("[pessoa] Sintaxe invalida: %s\n", atual->descricao);
            } else {
                removidos = RemoverPorParametro_pessoa(p, c.campo_filtro, c.valor_filtro);
                if (removidos < 0) {
                    printf("[pessoa] Campo %s nao pode ser usado no WHERE.\n", c.campo_filtro);
                } else {
                    printf("[pessoa] %d registro(s) removido(s) (%s = %d)\n",
                           removidos, c.campo_filtro, c.valor_filtro);
                    ok = 1;
                }
            }
        } else {
            printf("[pessoa] Processo desconhecido: %s\n", atual->nome);
        }

        if (!ok) {
            erros++;
        }
        free(atual);
    }

    return erros;
}
