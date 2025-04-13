
//listadefavoritos.h
#ifndef LISTADEFAVORITOS_H
#define LISTADEFAVORITOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "databasetomemory.h"
#include "databasetools.h"

void remover_id_da_lista(char *lista, int id_remover);
int id_ja_existe(const char *lista, int id);
void criar_lista_personalizada_favoritos();
void consultar_lista_personalizada_ou_favoritos();
void editar_ou_apagar_lista_personalizada_ou_favorito();

#endif