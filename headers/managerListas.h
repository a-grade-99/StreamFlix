
//listadefavoritos.h
#ifndef LISTADEFAVORITOS_H
#define LISTADEFAVORITOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storageManager.h"
#include "dataUtils.h"

void removerIdDaLista(char *lista, int id_remover);
int idExiste(const char *lista, int id);
void criarLista();
void consultarLista();
void editarOuApagarLista();

#endif