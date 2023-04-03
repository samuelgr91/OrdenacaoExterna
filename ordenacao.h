#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
    long matricula;
    double nota;
    char estado[4];
    char cidade[52];
    char curso[40];
}TipoRegistro;

typedef struct{
    TipoRegistro aluno;
    int bloco;

}TipoArea;


typedef struct{
    int escrita,leitura,comparacoes;
    

}TipoEstatistica;



int primeiroTemMaisPrioridade(const TipoRegistro* player1, const TipoRegistro* player2,TipoEstatistica *estatistica);
void troca(TipoRegistro* players, int p1, int p2) ;
void heapRefaz(TipoRegistro* registros, int i, int dir,TipoEstatistica *estatistica);
void heapConstroi(TipoRegistro* registros, int n,TipoEstatistica *estatistica) ;
void heap(TipoRegistro* registros, int n,TipoEstatistica *estatistica) ;
int heapCheio(TipoRegistro *registros,TipoEstatistica *estatistica);
void escreverFita(FILE **fita,int verificarFita, TipoRegistro *registros,TipoEstatistica *estatistica);
void fecharFita(FILE **fita,int verificarFita,TipoEstatistica *estatistica);
void desmarcarArea(TipoRegistro *registros);
void subSelecao(FILE **fita,FILE *f, int qtd,TipoEstatistica *estatistica);
void imprimirFitas(FILE **fita);


void heapRefazTipoArea(TipoArea* registros, int i, int dir,TipoEstatistica *estatistica);
void heapConstroiTipoArea(TipoArea* registros, int n,TipoEstatistica *estatistica) ;
void heapTipoArea(TipoArea* registros, int n,TipoEstatistica *estatistica) ;
int leProximoItem(TipoArea *area,FILE **vetorDeFitas,int fitaDeLeitura,TipoEstatistica *estatistica);
void escreverFitaSaida(TipoArea *area, FILE **vetorDeFitas,int posicao,TipoEstatistica *estatistica);
int heapSaidaCheio(TipoArea *registros,int qtdBlocos,TipoEstatistica *estatistica);
void intercalacao(FILE **vetorDeFitas,TipoEstatistica *estatistica);


int heapCheiof1(TipoRegistro *registros,TipoEstatistica *estatistica) ;
void escreverFitaf1(FILE **fita,int verificarFita, TipoRegistro *registros,TipoEstatistica *estatistica);
void fecharFitaf1(FILE **fita,int verificarFita,TipoEstatistica *estatistica);
void desmarcarAreaf1(TipoRegistro *registros);
void subSelecaof1(FILE ** fita,FILE *f,int qtd,TipoEstatistica *estatistica);
void trocaTipoAreaf1(TipoArea *vetor, int p1, int p2);
int primeiroTemMaisPrioridadeTipoAreaf1(const TipoArea* area1, const TipoArea* area2,TipoEstatistica *estatistica);
void heapRefazTipoAreaf1(TipoArea *areas, int i, int dir,TipoEstatistica *estatistica);
void heapConstroiTipoAreaf1(TipoArea *vetor, int n,TipoEstatistica *estatistica);
void heapTipoAreaf1(TipoArea *vetor, int n,TipoEstatistica *estatistica);
int leProximoItemf1(TipoArea *area,FILE **vetorDeFitas,int fitaDeLeitura,TipoEstatistica *estatistica);
void escreverFitaSaidaf1(TipoArea *area, FILE **vetorDeFitas,int posicao,TipoEstatistica *estatistica);
void escreverFitaLeituraf1(FILE **vetorDeFitas,TipoEstatistica *estatistica);
int heapSaidaCheiof1(TipoArea *registros,int qtdBlocos,TipoEstatistica *estatistica);
void limparFitasProxf1(FILE **vetorDeFitas,int fitaDeEscrita);
void intercalacaof1(FILE **vetorDeFitas,TipoEstatistica *estatistica);

