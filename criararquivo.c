#include "ordenacao.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>



void strtrim(char *str) {
    int i;
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) start++; // encontra o primeiro caractere não branco
    while ((end >= start) && isspace(str[end])) end--; // encontra o último caractere não branco
    for (i = start; i <= end; i++) {
        str[i-start] = str[i]; // move os caracteres não brancos para o início da string
    }
    str[i-start] = '\0'; // adiciona o caractere nulo no final da string
}



int main(){
    //uso o metodo 2f fitas no provao original para gerar cada arquivo com quantidade padrao ordenada crescentemente, para o decrescente leio de tras pra frente 
    //o arquivo crescente e salvo em outro arquivo

    FILE *orig, *dest;
    TipoRegistro r;
    long int tamanho;

    // abre o arquivo original para leitura
    orig = fopen("CRESCENTE471705.bin", "rb");
    if (orig == NULL) {
        printf("Erro ao abrir o arquivo original.\n");
        exit(1);
    }

    // cria o novo arquivo para escrita
    dest = fopen("DECRESCENTE471705.bin", "wb");
    if (dest == NULL) {
        printf("Erro ao criar o arquivo decrescente.\n");
        exit(1);
    }

    // posiciona o ponteiro no final do arquivo original
    fseek(orig, 0, SEEK_END);
    tamanho = ftell(orig);

    // loop para ler e escrever os registros
    while (tamanho >= sizeof(TipoRegistro)) {
        // move o ponteiro para o registro anterior
        tamanho -= sizeof(TipoRegistro);
        fseek(orig, tamanho, SEEK_SET);

        // lê o registro
        fread(&r, sizeof(TipoRegistro), 1, orig);

        // escreve o registro no arquivo decrescente
        fwrite(&r, sizeof(TipoRegistro), 1, dest);
    }

    // fecha os arquivos
    fclose(orig);
    fclose(dest);

    return 0;
}


