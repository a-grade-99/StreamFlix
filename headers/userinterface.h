//userinterface.h
#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Função para ler e validar opções do menu
int lerOpcao(int min, int max);

// Função de login corrigida
void menu_login(char *utilizador, unsigned int *senha);

// Menus aprimorados
int menu_principal(char *utilizador);
int menu_edicao_de_conteudos();
int menu_de_pesquisa();
int menu_lista_favoritos();
int menu_de_metricas();
int menu_player();

#endif