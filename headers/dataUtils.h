#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int tituloExiste(char titulo[100]);
int gerarId();
void adicionarOuEditarFilme(char titulo[100], char categoria, unsigned int duracao, unsigned int classificacao);
void apagarFilme(char titulo[100]);
void adicionarOuEditarUser(int id, char nome[10], int idade, unsigned int senha, char status, int atividade,
    char historico[1000], char personalizada[1000], char favoritos[1000]);

#endif