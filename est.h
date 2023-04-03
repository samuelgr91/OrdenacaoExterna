#ifndef EST_H
#define EST_H
#include <time.h>

typedef struct ests
{
    int num_transferencias_leitura;
    int num_transferencias_escrita;
    int num_comparacoes;
    clock_t time_inicioExecucao;
    clock_t time_finalExecucao;
    double total_execucao;
} ests;

ests *criaEstatistica();
void mostrarEstatistica(ests *est);
void incTransferenciaLeitura(ests *est);
void incTransferenciaEscrita(ests *est);
void incComparacao(ests *est);
void iniciaTimeExecucao(ests *est);
void encerraTimeExecucao(ests *est);
void calcTimeExecucao(ests *est);
#endif