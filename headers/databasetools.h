
//databasetools.h
#ifndef DATABASETOOLS_H
#define DATABASETOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int titulo_existe(char titulo[100]);
int gerar_id_unico();
void gravar_dados_filmes_CSV(char titulo[100], char categoria, unsigned int duracao, unsigned int classificacao);
void apagar_dados_filmes_CSV(char titulo[100]);
void gravar_dados_utilizador_CSV(int id, char nome[10], int idade, unsigned int senha, char status, int atividade,
    char historico[1000], char personalizada[1000], char favoritos[1000]);

#endif