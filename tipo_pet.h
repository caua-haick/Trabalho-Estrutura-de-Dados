//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_TIPO_PET_H
#define TRABALHO_AJALMAR_TIPO_PET_H
typedef struct tipo_pet {
    int codigo;
    char nome[50];
    struct tipo_pet *prox;
    struct tipo_pet *ant;
} tipo_pet;

int inserir_processos_tipo_pet();

int inserir_tipo_pet(tipo_pet **p);

int remover_tipo_pet(tipo_pet **p);


#endif //TRABALHO_AJALMAR_TIPO_PET_H
