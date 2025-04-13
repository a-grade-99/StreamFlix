#include <stdio.h>
#include "mainInterface.h" 
#include "edicaoFilmes.h"
#include "storageManager.h"
#include "managerPesquisa.h"
#include "managerListas.h"
#include "metricas.h"
#include "historicoFilmes.h"

int main() {
    char utilizador[10];
    unsigned int senha;

    guardarFilmesCsv();
    
    while(1){
        // Chama o menu de login
        interfaceLogin(utilizador, &senha);
        if(autenticarUtilizadorCsv(utilizador, senha) == 1){
            break;
        }
    }

    while(1){

        // Exibe o menu principal e pega a opção escolhida
        int opcao = interfaceMenuPrincipal(utilizador_logado->utilizador);

        // Processa a escolha do usuário
        switch (opcao) {
            case 1:
                if (utilizador_logado->status != 'a'){break;}
                opcao = interfaceEdicaoFIlmes();
                switch (opcao){
                    case 1:
                        limparListaFilmes();
                        interfaceAdicionarOuEditarFilme();
                        guardarFilmesCsv();
                        break;
                    case 2:
                        limparListaFilmes();
                        interfaceRemoverFilme();
                        guardarFilmesCsv();
                        break;
                }
                break;
            case 2:
                opcao = interfaceMenuPesquisa();
                switch (opcao){
                    case 1:
                        pesquisarPorTitulo();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        pesquisarPorCategoria();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        pesquisarPorFaixaEtaria();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;
            case 3:
                opcao = interfaceMenuFavoritos();
                switch (opcao){
                    case 1:
                        criarLista();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        consultarLista();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        editarOuApagarLista();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;
            case 4:
                opcao = interfaceMetricas();
                switch (opcao){
                    case 1:
                        filmesAssistidos();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        categoriasPopulares();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        usersAtivos();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;

            case 5:
                opcao = interfaceAssistir();
                switch (opcao){
                    case 1:
                        selecionarFilme();
                        break;
                    case 2:
                        listarHistorico();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        filmesRecomendados();
                        utilizador_logado->atividade ++;
                        break;
                }

                break;

            case 0:
                adicionarOuEditarUser(
                    utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
                    utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
                    utilizador_logado->personalizada, utilizador_logado->favoritos
                );
        
                limparListaFilmes();
                system("cls");
                printf("\n\n========== Obrigado Por Escolher a Streamflix, Volte Sempre :) ==========\n\n");
                printf("\n");
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
}