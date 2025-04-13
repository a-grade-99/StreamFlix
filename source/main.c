#include <stdio.h>
#include "userinterface.h" 
#include "edicaodeconteudo.h"
#include "databasetomemory.h"
#include "pesquisadeconteudo.h"
#include "listadefavoritos.h"
#include "metricas.h"
#include "player.h"

int main() {
    char utilizador[10];
    unsigned int senha;

    carregar_na_memoria_filmes_CSV();
    
    while(1){
        // Chama o menu de login
        menu_login(utilizador, &senha);
        if(carregar_na_memoria_utilizador_CSV(utilizador, senha) == 1){
            break;
        }
    }

    while(1){

        // Exibe o menu principal e pega a opção escolhida
        int opcao = menu_principal(utilizador_logado->utilizador);

        // Processa a escolha do usuário
        switch (opcao) {
            case 1:
                if (utilizador_logado->status != 'a'){break;}
                opcao = menu_edicao_de_conteudos();
                switch (opcao){
                    case 1:
                        liberar_memoria_filmes();
                        adicionar_editar_conteudo();
                        carregar_na_memoria_filmes_CSV();
                        break;
                    case 2:
                        liberar_memoria_filmes();
                        remover_conteudo();
                        carregar_na_memoria_filmes_CSV();
                        break;
                }
                break;
            case 2:
                opcao = menu_de_pesquisa();
                switch (opcao){
                    case 1:
                        pesquisar_por_titulo();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        pesquisar_por_categoria();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        pesquisar_por_faixa_etaria();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;
            case 3:
                opcao = menu_lista_favoritos();
                switch (opcao){
                    case 1:
                        criar_lista_personalizada_favoritos();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        consultar_lista_personalizada_ou_favoritos();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        editar_ou_apagar_lista_personalizada_ou_favorito();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;
            case 4:
                opcao = menu_de_metricas();
                switch (opcao){
                    case 1:
                        conteudos_mais_assistidos();
                        utilizador_logado->atividade ++;
                        break;
                    case 2:
                        categorias_mais_populares();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        utilizadores_mais_ativos();
                        utilizador_logado->atividade ++;
                        break;
                }
                break;

            case 5:
                opcao = menu_player();
                switch (opcao){
                    case 1:
                        selecionar_para_assistir();
                        break;
                    case 2:
                        listar_historico();
                        utilizador_logado->atividade ++;
                        break;
                    case 3:
                        filmes_recomendados();
                        utilizador_logado->atividade ++;
                        break;
                }

                break;

            case 0:
                gravar_dados_utilizador_CSV(
                    utilizador_logado->id, utilizador_logado->utilizador, utilizador_logado->idade, utilizador_logado->senha,
                    utilizador_logado->status, utilizador_logado->atividade, utilizador_logado->historico,
                    utilizador_logado->personalizada, utilizador_logado->favoritos
                );
        
                liberar_memoria_filmes();
                system("cls");
                printf("\n\n========== Obrigado Por Escolher a Streamflix, Volte Sempre :) ==========\n\n");
                printf("\n");
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
}