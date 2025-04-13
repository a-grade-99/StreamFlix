/**
* tests.h - Testes unitários e de integração para o sistema Streamflix
* Contém funções para testar os diferentes módulos do sistema
*/
#ifndef TESTS_H
#define TESTS_H

#include "streamflix.h"
#include "conteudo.h"
#include "user.h"
#include "ficheiroIO.h"
#include "recomendacoes.h"
#include "relatorios.h"

// Estrutura para resultados de teste
typedef struct {
    int totalTests;
    int passedTests;
    int failedTests;
} TestResults;

// Funções de teste para cada módulo
TestResults runContentTests();
TestResults runUserTests();
TestResults runFileIOTests();
TestResults runRecommendationTests();
TestResults runReportTests();

// Funções de teste de integração
TestResults runIntegrationTests();

// Função para executar todos os testes
TestResults runAllTests();

// Função para exibir resultados dos testes
void displayTestResults(TestResults results, const char* testName);

#endif // TESTS_H