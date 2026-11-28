#include <stdio.h>
#include "processo/processo.h"
#include "pessoa/pessoa.h"
#include "pet/pet.h"
#include "tipo_pet/tipo_pet.h"

int main() {
    char *script = "script.txt";
    processo *fila_geral = NULL;
    processo *fila_pessoa = NULL;
    processo *fila_pet = NULL;
    processo *fila_tipo_pet = NULL;
    lista_pessoa *pessoas = NULL;
    lista_pet *pets = NULL;
    lista_tipo_pet *tipos = NULL;
    int erros = 0;

    if (!ler_script(script, &fila_geral)) {
        return 1;
    }
    distribuir_processos(&fila_geral, &fila_pessoa, &fila_pet, &fila_tipo_pet);

    carregar_pessoa(&pessoas, ARQ_PESSOA);
    carregar_tipo_pet(&tipos, ARQ_TIPO_PET);
    carregar_pet(&pets, ARQ_PET);

    erros += realizar_processo_pessoaa(&pessoas, &fila_pessoa);
    erros += Realizar_Processo_Tipo_Pet(&tipos, &fila_tipo_pet);
    erros += Realizar_Processo_Pet(&pets, &fila_pet);

    salvar_pessoa(&pessoas, ARQ_PESSOA);
    salvar_tipo_pet(&tipos, ARQ_TIPO_PET);
    salvar_pet(&pets, ARQ_PET);

    liberar_pessoa(&pessoas);
    liberar_tipo_pet(&tipos);
    liberar_pet(&pets);

    printf("\nFim: %d processo(s) com erro.\n", erros);
    return 0;
}
