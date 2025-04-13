#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas Globais
typedef struct Filme {
    int id;
    char titulo[100];
    char categoria;
    unsigned int duracao;
    unsigned int classificacao;
    unsigned int visto;
    struct Filme *prox;  // Ponteiro para o próximo nó
} Filme;

typedef struct Utilizador{
    int id;
    char utilizador[10];
    int idade;
    unsigned int senha;
    char status;
    int atividade;
    char historico[1000];
    char personalizada[1000];
    char favoritos[1000];
} Utilizador;

// Declaração da variável global lista_filmes
extern Filme *lista_filmes;
extern Utilizador *utilizador_logado;

void guardarFilmesCsv();
void limparListaFilmes();
int autenticarUtilizadorCsv(char utilizador_digitado[10], unsigned int senha_digitada);

#endif