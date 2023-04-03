#include <stdio.h>
#include <stdlib.h>
#include "est.h"

/*
    A função mostrarests possui alguns códigos de cores da seguinte forma
    - x1b[31m = Vermelho
    - x1b[32m = Verde
    - x1b[33m = Amarelo
    - x1b[34m = Azul
    - x1b[0m = Reseta as cores após o uso de determinada cor (serve pra evitar de todo terminal tornar-se uma cor só)
*/

ests *criaEstatistica() // Inicia estrutura das estatísticas
{
    ests *nova = malloc(sizeof(ests));

    nova->num_comparacoes = 0;
    nova->num_transferencias_escrita = 0;
    nova->num_transferencias_leitura = 0;
    nova->total_execucao = 0.0;

    return nova;
}
void mostrarEstatistica(ests *est) // Imprime resultados das estatísticas
{
    printf("\n");
    printf("+-----------------------------+\n");
    printf("|     ESTATISTICAS FINAIS     |\n");
    printf("+-----------------------------+\n");
    printf("\n");

    printf("Numero de Transferencias (Leitura):\x1b[31m %d \x1b[0m\n", est->num_transferencias_leitura);
    printf("Numero de Transferencias (Escrita):\x1b[32m %d \x1b[0m\n", est->num_transferencias_escrita);
    printf("Numero Total de Comparacoes:\x1b[33m %d \x1b[0m\n", est->num_comparacoes);
    printf("Tempo Total Decorrido (milissegundos):\x1b[34m %lf \x1b[0m\n", est->total_execucao);

    printf("\n");
}

void incTransferenciaLeitura(ests *est) // Incrementa número de transferências de leitura
{
    est->num_transferencias_leitura++;
}

void incTransferenciaEscrita(ests *est) // Incrementa número de transferências de escrita
{
    est->num_transferencias_escrita++;
}

void incComparacao(ests *est) // Incrementa número de comparações
{
    est->num_comparacoes++;
}

void iniciaTimeExecucao(ests *est) // Inicia variável de tempo de execução
{
    est->time_inicioExecucao = clock();
}

void encerraTimeExecucao(ests *est) // Encerra variável de tempo de execução
{
    est->time_finalExecucao = clock();
    calcTimeExecucao(est);
}

void calcTimeExecucao(ests *est) // Calculo do tempo total, em milissegundos, do tempo de execução
{
    est->total_execucao = (est->time_finalExecucao - est->time_inicioExecucao) * 1000.0 / CLOCKS_PER_SEC;
}