#include "ordenacao.h"
#include "quickSortExterno.h"
#include "est.h"
int main(int argc,char *argv[]) {
    clock_t start, end;
    double elapsed;
    start = clock();
    //int metodo=1;
    int metodo =atoi(argv[1]);
    int qtd =atoi(argv[2]);
    int situacao =atoi(argv[3]);
    //int chave =atoi(argv[4]);
    
    
    
    //int *chaves=gerarChaves(qtd,situacao);
    TipoEstatistica *estatisticas=malloc(sizeof(TipoEstatistica));

    estatisticas->comparacoes=0;
    estatisticas->escrita=0;
    estatisticas->leitura=0;
    FILE *f;
    

    if( (metodo > 3 || metodo < 1) || (qtd > 471705 || qtd < 1) || (situacao > 3 || situacao < 1)){
        printf(" Entrada invalida!! \n ");
        return 0;
    }
        
    
    if(situacao == 1){
            if(qtd == 100)
                f = fopen("CRESCENTE100.bin","rb");
            else if(qtd == 200)
                f = fopen("CRESCENTE200.bin","rb"); 
            else if(qtd == 2000)
                f = fopen("CRESCENTE2000.bin","rb"); 
            else if(qtd == 20000)
                f = fopen("CRESCENTE20000.bin","rb");  
            else if(qtd == 200000)
                f = fopen("CRESCENTE200000.bin","rb");  
            else if(qtd == 471705)
                f = fopen("CRESCENTE471705.bin","rb");
            else
                f = fopen("CRESCENTE.bin","rb");
            }
    else if(situacao == 2){
        if(qtd == 100)
            f = fopen("DECRESCENTE100.bin","rb");
        else if(qtd == 200)
            f = fopen("DECRESCENTE200.bin","rb"); 
        else if(qtd == 2000)
            f = fopen("DECRESCENTE2000.bin","rb"); 
        else if(qtd == 20000)
            f = fopen("DECRESCENTE20000.bin","rb");  
        else if(qtd == 200000)
            f = fopen("DECRESCENTE200000.bin","rb");  
        else if(qtd == 471705)
            f = fopen("DECRESCENTE471705.bin","rb");
        else
            f = fopen("DECRESCENTE.bin","rb");
    }
    else{
        if((f = fopen("PROVAO.bin","rb")) == NULL){
            printf("Erro na abertura do arquivo\n");
            return 0;
        }
    }
    
    if((argc == 5) &&  strcmp(argv[4],"-P") == 0){
        TipoRegistro aux;
        int lidos=0;
        printf("\n\n\nArquivo Desordenado: \n");
        while(fread(&aux,sizeof(TipoRegistro),1,f) && lidos < qtd){
            if(aux.nota != 999.0)
            printf("Matricula: %ld      Nota: %lf    Estado: %s     Cidade: %s    Curso: %s\n",aux.matricula,aux.nota,aux.estado,aux.cidade,aux.curso);
            lidos++;
        }
        printf("\n\n\nArquivo Ordenado: \n");
        rewind(f);
    }
    

    char filename[20];
    FILE *fita[20];
    //FILE *saida[10];
    
    // abre os arquivos de entrada
    for (int i = 0; i < 20; i++) {
        sprintf(filename, "fita%d.bin", i);
        fita[i] = fopen(filename, "wb+");
        if (fita[i] == NULL) {
            printf("Erro ao criar arquivo %s\n", filename);
            exit(1);
        }
    }

    switch (metodo)
    {
    case 1:
    {
        subSelecao(fita,f,qtd,estatisticas);
        for(int i=0;i<20;i++)
            rewind(fita[i]);
        intercalacao(fita,estatisticas);
        if((argc == 5) &&  strcmp(argv[4],"-P") == 0)
            imprimirFitas(fita);

        for(int i=0;i<20;i++)
            fclose(fita[i]);
        break;
    }

    case 2:
    {
        
        subSelecaof1(fita,f,qtd,estatisticas);
        for(int i=0;i<20;i++)
            rewind(fita[i]);
        intercalacaof1(fita,estatisticas);
        if((argc == 5) &&  strcmp(argv[4],"-P") == 0)
            imprimirFitas(fita);

        
        
        break;
    }
    case 3:
    {
            FILE *arqLi,*arqEi, *arqLEs;
            ests *estatistica = malloc(sizeof(ests));
            arqLi = fopen("PROVAO2.bin", "r+b");
            arqEi = fopen("PROVAO2.bin", "r+b");
            arqLEs = fopen("PROVAO2.bin", "r+b");

            iniciaTimeExecucao(estatistica);
            QuickSortExterno(&arqLi, &arqEi, &arqLEs, 1, qtd, estatistica);
            encerraTimeExecucao(estatistica);
            mostrarEstatistica(estatistica);
            //fclose(arqEi);
            //fclose(arqLEs);
            rewind(f);
            break;
    }
}
    

    end = clock();

    elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo de execucao: %f segundos\nNumero de comparacoes: %d\nNumero de leitura em arquivo: %d\nNumero de escrita em arquivo: %d\n", elapsed,estatisticas->comparacoes,estatisticas->leitura,estatisticas->escrita);
        
    return 0;
}
