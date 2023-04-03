#ifndef quickSortExterno_h
#define quickSortExterno_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "ordenacao.h"
#include "est.h"

#define TAMAREA 10

typedef struct
{
    TipoRegistro aluno[TAMAREA];
} TipoAreaQuick;

#define TRUE 1
#define FALSE 0

void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, ests *estatisticas);
void FAVazia(TipoAreaQuick *Area);
int ObterNumCelOcupadas(TipoAreaQuick *Area, ests *estatisticas);
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer, ests *estatisticas);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer, ests *estatisticas);
void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es, ests *estatisticas);
void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei, ests *estatisticas);
void RetiraMax(TipoAreaQuick *Area, TipoRegistro *R, int *NRArea, ests *estatisticas);
void RetiraMin(TipoAreaQuick *Area, TipoRegistro *R, int *NRArea, ests *estatisticas);
void RetiraUltimo(TipoAreaQuick *Area, TipoRegistro *R, ests *estatisticas);
void RetiraPrimeiro(TipoAreaQuick *Area, TipoRegistro *R, ests *estatisticas);
void InsereItem(TipoRegistro UltLido, TipoAreaQuick *Area, ests *estatisticas);
void InserirArea(TipoAreaQuick *Area, TipoRegistro *UltLido, int *NRArea, ests *estatisticas);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoAreaQuick *Area, int Esq, int Dir, int *i, int *j, ests *estatisticas);
void imprimirArea(TipoAreaQuick *Area);
void imprimeAlunoQuick(TipoRegistro *Area);
#endif