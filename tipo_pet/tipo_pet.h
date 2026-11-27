//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_TIPO_PET_H
#define TRABALHO_AJALMAR_TIPO_PET_H

typedef struct tipo_pet {
    int codigo;
    char nome[50];
} tipo_pet;

typedef struct lista_tipo_pet {
    tipo_pet dados;
    struct lista_tipo_pet *prox;
    struct lista_tipo_pet *ant;
} lista_tipo_pet;

int inserir_processos_tipo_pet();

// Alterado para receber ponteiro para lista_tipo_pet **
int inserir_tipo_pet(lista_tipo_pet **p, tipo_pet *np);

// Alterado para receber ponteiro para lista_tipo_pet **
int remover_tipo_pet(lista_tipo_pet **p);

#endif //TRABALHO_AJALMAR_TIPO_PET_H