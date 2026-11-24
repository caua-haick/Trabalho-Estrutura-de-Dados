//
// Created by caua2 on 24/11/2026.
//

#ifndef TRABALHO_AJALMAR_PET_H
#define TRABALHO_AJALMAR_PET_H
typedef struct pet {
    int codigo;
    int codigo_pessoa;
    char nome[50];
    int codigo_tipo_pet;
    struct pet *prox;
    struct pet *ant;
} pet;
#endif //TRABALHO_AJALMAR_PET_H
