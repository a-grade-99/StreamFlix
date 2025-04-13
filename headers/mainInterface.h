#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Função para ler e validar opções do menu
int lerOpcao(int min, int max);

// Função de login corrigida
void interfaceLogin(char *utilizador, unsigned int *senha);

// Menus aprimorados
int interfaceMenuPrincipal(char *utilizador);
int interfaceEdicaoFIlmes();
int interfaceMenuPesquisa();
int interfaceMenuFavoritos();
int interfaceMetricas();
int interfaceAssistir();

#endif