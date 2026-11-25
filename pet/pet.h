//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PET_H
#define TRABALHO_AJALMAR_PET_H
typedef struct processos_pet {
    char nome[50];
    char descricao[50];
    struct processos_pet *prox;
} processos_pet;

typedef struct pet {
    int codigo;
    int codigo_pessoa;
    char nome[50];
    int codigo_tipo_pet;
} pet;
typedef struct lista_pet{
    pet dados;
    struct pet *prox;
    struct pet *ant;
}lista_pet;

int inserir_pet(lista_pet **p,pet *np);

int remover_pet(lista_pet **p);

int inserir_processos_pet(processos_pet** p);

void Realizar_Processo(lista_pet **p);
#endif //TRABALHO_AJALMAR_PET_H
