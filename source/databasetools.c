
//databasetools.c
#include "databasetools.h"

#define FILE_PATH "filmes.csv"
#define TEMP_FILE "temp.csv"
#define UTILIZADOR_PATH "users.csv"
#define TEMP_FILE_UTILIZADOR "temp_user.csv"


// Função para verificar se um título já existe no CSV
int titulo_existe(char titulo[100]) {
    FILE *arquivo = fopen(FILE_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return -1;
    }

    char linha[200], titulo_existente[100];
    int id, categoria, duracao, classificacao, visto;

    // Ignora o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    // Verifica se o título já existe
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%99[^,],%d,%d,%d,%d", &id, titulo_existente, &categoria, &duracao, &classificacao, &visto);
        if (strcmp(titulo_existente, titulo) == 0) {
            fclose(arquivo);
            return 1; // O título já existe
        }
    }

    fclose(arquivo);
    return 0; // O título não existe
}

// Função para gerar um ID único
int gerar_id_unico() {
    FILE *arquivo = fopen(FILE_PATH, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return -1;
    }

    int ids_existentes[10000] = {0}; // Vetor para armazenar IDs já usados
    int id, max_id = 0;
    char linha[200];

    // Ignora o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    // Lê IDs existentes
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,", &id);
        if (id > max_id) {
            max_id = id;
        }
        ids_existentes[id] = 1;
    }

    fclose(arquivo);

    // Gera um novo ID único
    srand(time(NULL));
    do {
        id = max_id + 1; // Garante que o novo ID seja maior que o último existente
    } while (ids_existentes[id] == 1);

    return id;
}

void gravar_dados_filmes_CSV(char titulo[100], char categoria, unsigned int duracao, unsigned int classificacao) {
    FILE *arquivo = fopen(FILE_PATH, "r");
    FILE *temp = fopen(TEMP_FILE, "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }

    char linha[200], titulo_existente[100];
    int id, duracao_lida, classificacao_lida, visto;
    char categoria_lida;
    int encontrou = 0;
    int linhas_copiadas = 0;

    // Copia o cabeçalho
    if (fgets(linha, sizeof(linha), arquivo)) {
        fprintf(temp, "%s", linha);
        linhas_copiadas++;
        
        // Garante que o cabeçalho termina com uma quebra de linha
        if (linha[strlen(linha) - 1] != '\n') {
            fprintf(temp, "\n");
        }
    }

    // Percorre o CSV e atualiza se o título existir
    while (fgets(linha, sizeof(linha), arquivo)) {
        int lidos = sscanf(linha, "%d,%99[^,],%c,%d,%d,%d", &id, titulo_existente, &categoria_lida, &duracao_lida, &classificacao_lida, &visto);
        
        if (lidos != 6) {
            printf("Erro ao ler a linha: %s\n", linha);
            continue;
        }

        if (strcmp(titulo_existente, titulo) == 0) {
            // Atualiza os dados do filme existente
            fprintf(temp, "%d,%s,%c,%u,%u,%d\n", id, titulo, categoria, duracao, classificacao, visto);
            encontrou = 1;
        } else {
            // Mantém os outros filmes sem alterações
            fprintf(temp, "%s", linha);

            // Garante que cada linha termina com uma quebra de linha
            if (linha[strlen(linha) - 1] != '\n') {
                fprintf(temp, "\n");
            }
        }
    }

    // Se o título não existir, adiciona um novo filme com "Visto = 0"
    if (!encontrou) {
        // Se não houver linhas anteriores ou a última linha não termina com quebra de linha, adiciona uma
        if (linhas_copiadas > 0 && linha[strlen(linha) - 1] != '\n') {
            fprintf(temp, "\n");
        }
        int novo_id = gerar_id_unico();
        int visto_novo = 0;  // Filme novo começa com "visto = 0"
        fprintf(temp, "%d,%s,%c,%u,%u,%d\n", novo_id, titulo, categoria, duracao, classificacao, visto_novo);
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o CSV original pelo atualizado
    remove(FILE_PATH);
    rename(TEMP_FILE, FILE_PATH);
}

void apagar_dados_filmes_CSV(char titulo[100]) {
    FILE *db_filmes = fopen(FILE_PATH, "r");
    FILE *temp_file = fopen(TEMP_FILE, "w");

    if (db_filmes == NULL || temp_file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[200];
    int removido = 0;

    // Copiar o cabeçalho para o novo arquivo
    if (fgets(linha, sizeof(linha), db_filmes)) {
        fprintf(temp_file, "%s", linha);
    }

    while (fgets(linha, sizeof(linha), db_filmes)) {
        char id[10], titulo_filme[100], categoria[5], duracao[10], classificacao[10], visto[10];

        // Divide os campos do CSV, agora incluindo "Visto"
        int lidos = sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,\n]",
                           id, titulo_filme, categoria, duracao, classificacao, visto);

        if (lidos != 6) {
            printf("Erro ao ler a linha: %s\n", linha);
            continue; // Ignora linhas mal formatadas
        }

        // Se o título **não for** o que queremos apagar, copiamos para o novo ficheiro
        if (strcmp(titulo_filme, titulo) != 0) {
            fprintf(temp_file, "%s", linha);
        } else {
            removido = 1;
        }
    }

    fclose(db_filmes);
    fclose(temp_file);

    // Substitui o arquivo original pelo atualizado
    if (removido) {
        remove(FILE_PATH);
        rename(TEMP_FILE, FILE_PATH);
        printf("Filme \"%s\" removido com sucesso!\n", titulo);
    } else {
        remove(TEMP_FILE);
        printf("Filme \"%s\" não encontrado!\n", titulo);
    }
}

void gravar_dados_utilizador_CSV(int id, char nome[10], int idade, unsigned int senha, char status, int atividade,
                                 char historico[1000], char personalizada[1000], char favoritos[1000]) {
    FILE *arquivo = fopen(UTILIZADOR_PATH, "r");
    FILE *temp = fopen(TEMP_FILE_UTILIZADOR, "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }

    char linha[1200];
    int id_lido, idade_lida, atividade_lida;
    unsigned int senha_lida;
    char nome_lido[10], status_lido;
    char historico_lido[1000] = "", personalizada_lida[1000] = "", favoritos_lida[1000] = "";

    int linha_alterada = 0;

    // Copiar cabeçalho sem pular linha
    if (fgets(linha, sizeof(linha), arquivo)) {
        fprintf(temp, "%s", linha);
    }

    while (fgets(linha, sizeof(linha), arquivo)) {
        memset(nome_lido, 0, sizeof(nome_lido));
        memset(historico_lido, 0, sizeof(historico_lido));
        memset(personalizada_lida, 0, sizeof(personalizada_lida));
        memset(favoritos_lida, 0, sizeof(favoritos_lida));

        int itens_lidos = sscanf(linha, "%d,%9[^,],%d,%u,%c,%d,%999[^,],%999[^,],%999[^,\n]",
                                 &id_lido, nome_lido, &idade_lida, &senha_lida, &status_lido, &atividade_lida,
                                 historico_lido, personalizada_lida, favoritos_lida);

        if (itens_lidos < 6) {
            printf("ERRO: Linha inválida ignorada: %s\n", linha);
            continue;
        }

        if (id_lido == id && !linha_alterada) {
            fprintf(temp, "%d,%s,%d,%u,%c,%d,%s,%s,%s\n",
                    id, nome, idade, senha, status, atividade, historico, personalizada, favoritos);
            linha_alterada = 1;
        } else {
            fprintf(temp, "%s", linha);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove(UTILIZADOR_PATH);
    rename(TEMP_FILE_UTILIZADOR, UTILIZADOR_PATH);
}
