
#include "ordenacao.h"

void imprimirFitas(FILE **fita) {
    TipoRegistro aux;
    //int ctd=0;
    for(int i=0 ;i < 20; i++){
        rewind(fita[i]);
        //printf("fita%d :\n",i);
    
        while(fread(&aux, sizeof(TipoRegistro), 1, fita[i]) == 1) {
        if(aux.nota == 999.0 || aux.matricula == 0){
            printf("\n");
        }
        else{
            //ctd++;
            //printf("Nota: %lf\n", registro.nota);
            printf("Matricula: %ld      Nota: %lf    Estado: %s     Cidade: %s    Curso: %s\n",aux.matricula,aux.nota,aux.estado,aux.cidade,aux.curso); 
        }
    }
   
    }   
     //printf("%d \n", ctd);
      
}

int primeiroTemMaisPrioridade(const TipoRegistro* player1, const TipoRegistro* player2,TipoEstatistica *estatistica) {
        estatistica->comparacoes++;
        return player1->nota > player2->nota;
        
}


void troca(TipoRegistro* players, int p1, int p2) {
    TipoRegistro aux=players[p2];
    players[p2]=players[p1];
    players[p1]=aux;
    
}

void heapRefaz(TipoRegistro* players, int i, int dir,TipoEstatistica *estatistica) {
    int j=i*2+1;
    TipoRegistro aux=players[i];
    if(j > dir)
        return;
    if(j<dir && !primeiroTemMaisPrioridade(&players[j],&players[j+1],estatistica))
        j++;
    if(primeiroTemMaisPrioridade(&aux,&players[j],estatistica))
        return;
    
    troca(players,i,j);
    heapRefaz(players,j,dir,estatistica);
    
}


void heapConstroi(TipoRegistro* players, int n,TipoEstatistica *estatistica) {
    int esq=(n/2)-1;
    while(esq >= 0){
        heapRefaz(players,esq,n-1,estatistica);
        esq--;
    }
}


void heap(TipoRegistro* players, int n,TipoEstatistica *estatistica) {//heapsort aproveitado da disciplina Estrutura de Dados 1
    heapConstroi(players,n,estatistica);
    while(n>1){
        troca(players,0,n-1);
        n--;
        heapRefaz(players,0,n-1,estatistica);
    }
}



int heapCheio(TipoRegistro *registros,TipoEstatistica *estatistica) {//se algum valor nao tiver marcado retorna falso
    for (int i = 0; i < 10; i++){
        estatistica->comparacoes++;
        if (registros[i].nota < 101) {
            return 0;
        }
    }
    return 1;
}

void escreverFita(FILE **fita,int verificarFita, TipoRegistro *registros,TipoEstatistica *estatistica){
    if (registros[0].nota < 101){
        estatistica->comparacoes++;
        estatistica->escrita++;
        fwrite(&registros[0], sizeof(TipoRegistro), 1, fita[verificarFita]);      
    }
        
}



void fecharFita(FILE **fita,int verificarFita,TipoEstatistica *estatistica){//marca fim do bloco
    TipoRegistro aux;
    aux.matricula = 0;
    aux.nota = 999.0;
    aux.estado[0] = '*';
    aux.cidade[0] = '*';
    aux.curso[0] = '*';
    estatistica->escrita++;
    fwrite (&aux, sizeof(TipoRegistro), 1, fita[verificarFita]);
}

void desmarcarArea(TipoRegistro *registros){
    for (int i = 0; i < 10; i++)
        registros[i].nota-=101;
}




void subSelecao(FILE ** fita,FILE *f,int qtd,TipoEstatistica *estatistica){//substituicao por seleçao para 2f fitas
    


    TipoRegistro area[10];//area disponivel na memoria principal
    TipoRegistro aux;
    int verificaFita=0,lidos=0;
    
    for(int i=0;i<10;i++){//preenche area inicial
        estatistica->leitura++;
        fread(&area[i], sizeof(TipoRegistro), 1, f);
            
    }
        
    heap(area,10,estatistica);
    estatistica->escrita++;
    fwrite(&area[0], sizeof(TipoRegistro), 1,fita[0]);//escreve o primeiro menor na primeira fita 
    
    while(fread(&aux, sizeof(TipoRegistro), 1, f) && lidos < (qtd-10) ){//enquanto nao chegar na quantidade maxima digitada pelo usuario ou fim do arquivo
        estatistica->leitura++;
        if(!heapCheio(area,estatistica)){//heap cheio é quando todos itens estao marcados
            estatistica->comparacoes++;
            if(area[0].nota > aux.nota){//se entrar item menor do que o ultimo escrito
                area[0]=aux;
                area[0].nota+=101;//item marcado
            }
            else{
                area[0]=aux;
                
            }
            heap(area,10,estatistica);
            escreverFita(fita,verificaFita,area,estatistica);//escreve o menor            
        }
        else{//caso heap encheu
            fecharFita(fita,verificaFita,estatistica); //cria um registro para marcar o fim do bloco
            if(verificaFita == 9)//vai para o proximo bloco a ser escrito
                verificaFita=0;
            else
                verificaFita++;
            desmarcarArea(area);
            heap(area,10,estatistica);
            escreverFita(fita,verificaFita,area,estatistica);
            estatistica->comparacoes++;
            if(area[0].nota > aux.nota){//antes de ir para proxima iteraçao precisa tratar o ultimo lido para nao perde-lo 
                area[0]=aux;
                area[0].nota+=101;
            }
            else{
                area[0]=aux;
                
            }
            heap(area,10,estatistica);
            escreverFita(fita,verificaFita,area,estatistica); 
        }
        lidos++;
    }

    //salvar registros que sobraram na area
    heap(area,10,estatistica);
    for(int i=0;i<10;i++){//escreve na fita atual os itens nao marcados
        estatistica->comparacoes++;
        if(area[i].nota < 101){
            estatistica->escrita++;
            fwrite (&area[i], sizeof(TipoRegistro), 1, fita[verificaFita]);    
        }
    }
    fecharFita(fita,verificaFita,estatistica);

    //escreve os itens marcados na proxima fita
    if(verificaFita == 9)
        verificaFita=0;
    else
        verificaFita++;

    
    int cont=0;
    for(int i=0;i<10;i++){
        estatistica->comparacoes++;
        if(area[i].nota > 100){
            area[i].nota-=101;
            estatistica->escrita++;
            fwrite (&area[i], sizeof(TipoRegistro), 1, fita[verificaFita]);
            cont++; 
        }  
    }
    
    if(cont > 0)
        fecharFita(fita,verificaFita,estatistica);
    
    //fechar ultimas fitas escritas*/

}



void trocaTipoArea(TipoArea *vetor, int p1, int p2,TipoEstatistica *estatistica) {
    TipoArea aux = vetor[p1];
    vetor[p1] = vetor[p2];
    vetor[p2] = aux;
}

int primeiroTemMaisPrioridadeTipoArea(const TipoArea* area1, const TipoArea* area2,TipoEstatistica *estatistica) {
    estatistica->comparacoes++;
    return  area1->aluno.nota > area2->aluno.nota;
}

void heapRefazTipoArea(TipoArea *areas, int i, int dir,TipoEstatistica *estatistica) {
    int j = i*2+1;
    TipoArea aux = areas[i];
    if(j > dir)
        return;
    if(j < dir && !primeiroTemMaisPrioridadeTipoArea(&areas[j], &areas[j+1],estatistica))
        j++;
    if(primeiroTemMaisPrioridadeTipoArea(&aux, &areas[j],estatistica))
        return;
    
    trocaTipoArea(areas, i, j,estatistica);
    heapRefazTipoArea(areas, j, dir,estatistica);
}

void heapConstroiTipoArea(TipoArea *vetor, int n,TipoEstatistica *estatistica) {
    int esq = (n/2) - 1;
    while(esq >= 0) {
        heapRefazTipoArea(vetor, esq, n-1,estatistica);
        esq--;
    }
}

void heapTipoArea(TipoArea *vetor, int n,TipoEstatistica *estatistica) {//as ultimas funçoes sao heapsort para lidar com TAD do tipoArea
    heapConstroiTipoArea(vetor, n,estatistica);
    while(n > 1) {
        trocaTipoArea(vetor, 0, n-1,estatistica);
        n--;
        heapRefazTipoArea(vetor, 0, n-1,estatistica);
    }
}




int leProximoItem(TipoArea *area,FILE **vetorDeFitas,int fitaDeLeitura,TipoEstatistica *estatistica){//le proximo item do bloco onde o anterior foi retirado
    int posicao=area[0].bloco;
    estatistica->leitura++;
    if(fread(&area[0].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+posicao]) == 1){
            area[0].bloco=posicao;
            return 1;
        }
    else 
        return 0;
    
    
}

void escreverFitaSaida(TipoArea *area, FILE **vetorDeFitas,int posicao,TipoEstatistica *estatistica){
    //area[7].aluno.nota=9;
    estatistica->comparacoes++;
    estatistica->escrita++;
    if (area[0].aluno.nota != 999.0 )
        fwrite (&area[0].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[posicao]);
           
            
    
    
}


int heapSaidaCheio(TipoArea *registros,int qtdBlocos,TipoEstatistica *estatistica) {//nessa fase o heap cheio é quando leu todos blocos de uma intercalaçao
    for (int i = 0; i < qtdBlocos; i++){
        estatistica->comparacoes++;
        if (registros[i].aluno.nota != 999.0) {
            return 0;
        }
    }
    return 1;
}

void limparFitasProx(FILE **vetorDeFitas,int fitaDeEscrita){//limpa fitas que vao ser as proximas fitas de escrita
    char filename[20];
    for (int i = 0; i < 10; i++)
        fclose(vetorDeFitas[fitaDeEscrita+i]);
        
    if(fitaDeEscrita < 10){
        for (int i = 0; i < 10; i++) {
            sprintf(filename, "fita%d.bin", i);
            vetorDeFitas[i] = fopen(filename, "wb+");
            if (vetorDeFitas[i] == NULL) {
                printf("Erro ao criar arquivo %s\n", filename);
                exit(1);
            }
                }
    }
    else{
        for (int i = 10; i < 20; i++) {
            
            sprintf(filename, "fita%d.bin", i);
            vetorDeFitas[i] = fopen(filename, "wb+");
            if (vetorDeFitas[i] == NULL) {
                printf("Erro ao criar arquivo %s\n", filename);
                exit(1);
            }
                }
    }
}



void intercalacao(FILE **vetorDeFitas,TipoEstatistica *estatistica){//intercalaçao 2f fitas
        int fitaDeEscrita=10;
        int fitaDeLeitura=0;
    
        TipoArea area[10];
        

        int i=0,qtdBlocos=0;

        while(i < 10 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+i])){
            estatistica->leitura++;
            area[i].bloco=i;
            qtdBlocos++;
            i++;
        }
        int verificaFita=0;//iterador de escrita
       
        heapTipoArea(area,qtdBlocos,estatistica);

        escreverFitaSaida(area,vetorDeFitas,fitaDeEscrita+verificaFita,estatistica);
        
        
        
        while(1){  
              
                if(!heapSaidaCheio(area,qtdBlocos,estatistica)){//enquanto nao chegar no fim de todos blocos da fase de intercalaçao     
                    //printf("111");
                    if(leProximoItem(area,vetorDeFitas,fitaDeLeitura,estatistica)){
                        heapTipoArea(area,qtdBlocos,estatistica);
                        escreverFitaSaida(area,vetorDeFitas,fitaDeEscrita+verificaFita,estatistica);
                    }
                    else{//se nao ler o proximo item é porque esta no fim da fita
                        for(int k=1;k<qtdBlocos;k++)
                            area[k-1]=area[k];
                        qtdBlocos--;
                        heapTipoArea(area,qtdBlocos,estatistica);
                        escreverFitaSaida(area,vetorDeFitas,fitaDeEscrita+verificaFita,estatistica);
                    }
                            
                }
                else{
                    
                    fecharFita(vetorDeFitas,fitaDeEscrita+verificaFita,estatistica);
                    
                    i=0;
                    qtdBlocos=0;
                    while(i < 10 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+i])){//verifica se tem blocos na entrada para serem intercalados
                        estatistica->leitura++;
                        area[i].bloco=i;
                        i++;
                        qtdBlocos++;
                    }  
                    if(qtdBlocos > 0){
                        
                        if(verificaFita == 9)
                            verificaFita=0;
                        else
                            verificaFita++;

                        heapTipoArea(area,qtdBlocos,estatistica);
                        escreverFitaSaida(area,vetorDeFitas,fitaDeEscrita+verificaFita,estatistica);
                    }
                    else{//fitas de leitura nao tem mais registros
                        i=0;
                        qtdBlocos=0;
                        verificaFita=0;
                        for(int k=0;k<10;k++)
                            rewind(vetorDeFitas[fitaDeEscrita+k]);
                        
                        
                        while(i < 10 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeEscrita+i])){//verifica se precisa intercalar as fitas de saida
                                estatistica->leitura++;
                                area[i].bloco=i;
                                i++;
                                qtdBlocos++;
                            }
                       
                        if(qtdBlocos > 1){//muda fita de escrita com leitura e vice versa
                            limparFitasProx(vetorDeFitas,fitaDeLeitura);
                            verificaFita=0;
                            int aux=fitaDeEscrita;
                            fitaDeEscrita=fitaDeLeitura;
                            fitaDeLeitura=aux;
                            heapTipoArea(area,qtdBlocos,estatistica);
                            escreverFitaSaida(area,vetorDeFitas,fitaDeEscrita+verificaFita,estatistica); 
                        }
                        else if(qtdBlocos == 1){//todos blocos intercalados
                            limparFitasProx(vetorDeFitas,fitaDeLeitura);
                            break;
                        }         
                    }        
            }    
        }
}



int heapCheiof1(TipoRegistro *registros,TipoEstatistica *estatistica) {//verifica se area chegou no fim de todos blocos da intercalaçao atual
    for (int i = 0; i < 19; i++){
        estatistica->comparacoes++;
        if (registros[i].nota < 101) {
            return 0;
        }
    }
    return 1;
}

void escreverFitaf1(FILE **fita,int verificarFita, TipoRegistro *registros,TipoEstatistica *estatistica){
    if (registros[0].nota < 101){
        estatistica->escrita++;
        estatistica->comparacoes++;
        fwrite(&registros[0], sizeof(TipoRegistro), 1, fita[verificarFita]);
        
    }
}



void fecharFitaf1(FILE **fita,int verificarFita,TipoEstatistica *estatistica){//marca o fim do bloco com um registro ficticio
   
    TipoRegistro aux;
    aux.matricula = 0;
    aux.nota = 999.0;
    aux.estado[0] = '*';
    aux.cidade[0] = '*';
    aux.curso[0] = '*';
    estatistica->escrita++;
    fwrite (&aux, sizeof(TipoRegistro), 1, fita[verificarFita]);
}

void desmarcarAreaf1(TipoRegistro *registros){
    for (int i = 0; i < 19; i++)
        registros[i].nota-=101;
}




void subSelecaof1(FILE ** fita,FILE *f,int qtd,TipoEstatistica *estatistica){
    
    TipoRegistro area[19];
    TipoRegistro aux;
    int verificaFita=0,lidos=0;
    
    for(int i=0;i<19;i++){//preenche area com 19 registros iniciais
        estatistica->leitura++;
        fread(&area[i], sizeof(TipoRegistro), 1, f);        
    }
        
    heap(area,19,estatistica);
    estatistica->escrita++;
    fwrite(&area[0], sizeof(TipoRegistro), 1,fita[0]);//escreve o primeiro menor na primeira fita 
    
    while(fread(&aux, sizeof(TipoRegistro), 1, f) && lidos < (qtd-19)){
        estatistica->leitura++;
        if(!heapCheiof1(area,estatistica)){//heap cheio é quando leu todos blocos da intercalaçao atual
            estatistica->comparacoes++;
            if(area[0].nota > aux.nota){//marca valor quando é menor do que o ultimo escrito em alguma fita
                area[0]=aux;
                area[0].nota+=101;
            }
            else{
                area[0]=aux; 
            }
            heap(area,19,estatistica);
            escreverFitaf1(fita,verificaFita,area,estatistica);            
        }
        else{
            fecharFitaf1(fita,verificaFita,estatistica);//registro ficticio para marcar fim de uma intercalaçao
            if(verificaFita == 18)
                verificaFita=0;
            else
                verificaFita++;
            desmarcarAreaf1(area);
            heap(area,19,estatistica);
            escreverFitaf1(fita,verificaFita,area,estatistica);//escreve o menor do novo bloco
            estatistica->comparacoes++;
            if(area[0].nota > aux.nota){//trata o ultimo valor lido do arquivo
                area[0]=aux;
                area[0].nota+=101;
            }
            else{
                area[0]=aux;
                
            }
            heap(area,19,estatistica);
            escreverFitaf1(fita,verificaFita,area,estatistica);   
        }
        lidos++;
    }

    //tratamento dos registros que ficam na area quando acaba a leitura do arquivo
    heap(area,19,estatistica);
    for(int i=0;i<19;i++){//grava valores nao marcados na fita atual
        if(area[i].nota < 101){
            estatistica->comparacoes++;
            estatistica->escrita++;
            fwrite (&area[i], sizeof(TipoRegistro), 1, fita[verificaFita]);    
        }
    }
    fecharFitaf1(fita,verificaFita,estatistica);


    if(verificaFita == 18)
        verificaFita=0;
    else
        verificaFita++;

    
    int cont=0;
    for(int i=0;i<19;i++){//grava valores marcados na proxima fita, desmarcados
        if(area[i].nota > 100 && area[i].nota < 202){
            estatistica->comparacoes++;
            area[i].nota-=101;
            estatistica->escrita++;
            fwrite (&area[i], sizeof(TipoRegistro), 1, fita[verificaFita]);
            cont++; 
        }  
    }
    
    if(cont > 0)
        fecharFitaf1(fita,verificaFita,estatistica);
    
}


void trocaTipoAreaf1(TipoArea *vetor, int p1, int p2) {
    TipoArea aux = vetor[p1];
    vetor[p1] = vetor[p2];
    vetor[p2] = aux;
}

int primeiroTemMaisPrioridadeTipoAreaf1(const TipoArea* area1, const TipoArea* area2,TipoEstatistica *estatistica) {
    estatistica->comparacoes++;
    return  area1->aluno.nota > area2->aluno.nota;
}

void heapRefazTipoAreaf1(TipoArea *areas, int i, int dir,TipoEstatistica *estatistica) {
    int j = i*2+1;
    TipoArea aux = areas[i];
    if(j > dir)
        return;
    if(j < dir && !primeiroTemMaisPrioridadeTipoAreaf1(&areas[j], &areas[j+1],estatistica))
        j++;
    if(primeiroTemMaisPrioridadeTipoAreaf1(&aux, &areas[j],estatistica))
        return;
    
    trocaTipoAreaf1(areas, i, j);
    heapRefazTipoAreaf1(areas, j, dir,estatistica);
}

void heapConstroiTipoAreaf1(TipoArea *vetor, int n,TipoEstatistica *estatistica) {
    int esq = (n/2) - 1;
    while(esq >= 0) {
        heapRefazTipoAreaf1(vetor, esq, n-1,estatistica);
        esq--;
    }
}

void heapTipoAreaf1(TipoArea *vetor, int n,TipoEstatistica *estatistica) {//funçoes do heapsort feitas na disciplina ED1
    heapConstroiTipoAreaf1(vetor, n,estatistica);
    while(n > 1) {
        trocaTipoAreaf1(vetor, 0, n-1);
        n--;
        heapRefazTipoAreaf1(vetor, 0, n-1,estatistica);
    }
}




int leProximoItemf1(TipoArea *area,FILE **vetorDeFitas,int fitaDeLeitura,TipoEstatistica *estatistica){//le o proximo item na fita onde o ultimo valor que foi escrito 
    int posicao=area[0].bloco;
    estatistica->leitura++;
    if(fread(&area[0].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+posicao]) == 1){
            area[0].bloco=posicao;
            return 1;
        }
    else 
        return 0;
    
    
}

void escreverFitaSaidaf1(TipoArea *area, FILE **vetorDeFitas,int posicao,TipoEstatistica *estatistica){
    if (area[0].aluno.nota != 999.0 ){
        estatistica->escrita++;
        fwrite (&area[0].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[posicao]);
    }
}

void escreverFitaLeituraf1(FILE **vetorDeFitas,TipoEstatistica *estatistica){//escreve valores da fita de saida nas entradas, quando tem mais de um bloco na saida
    int i=0;
    TipoRegistro tmp;
    while(fread(&tmp, sizeof(TipoRegistro), 1, vetorDeFitas[19])){
        estatistica->escrita++;
        estatistica->leitura++;
        fwrite(&tmp,sizeof(TipoRegistro),1,vetorDeFitas[i]);
        estatistica->comparacoes++;
        if(tmp.nota == 999.0){//caso seja o fim de um bloco tem que gravar os proximos na proxima fita
            if(i == 18)
                i=0;
            else
                i++;
        }
    }

    for(i=0;i<19;i++)
        rewind(vetorDeFitas[i]);
    
    //limpa a fita de saida
    fclose(vetorDeFitas[19]);
    char filename[20];
    sprintf(filename, "fita%d.bin", 19);
    vetorDeFitas[i] = fopen(filename, "wb+");
    if (vetorDeFitas[19] == NULL) {
        printf("Erro ao criar arquivo fita19\n");
        exit(1);
    }
           
            
    
    
}


int heapSaidaCheiof1(TipoArea *registros,int qtdBlocos,TipoEstatistica *estatistica) {//verifica se todos blocos da intercalaçao atual estao no final
    for (int i = 0; i < qtdBlocos; i++){
        if (registros[i].aluno.nota != 999.0) {
            return 0;
        }
    }
    return 1;
}

void limparFitasProxf1(FILE **vetorDeFitas,int fitaDeEscrita){//limpa fitas de entrada que vao receber novos valores
    char filename[20];
    for (int i = 0; i < 19; i++)
        fclose(vetorDeFitas[i]);
        
    
    for (int i = 0; i < 19; i++) {
        sprintf(filename, "fita%d.bin", i);
        vetorDeFitas[i] = fopen(filename, "wb+");
        if (vetorDeFitas[i] == NULL) {
            printf("Erro ao criar arquivo %s\n", filename);
            exit(1);
        }
    }
    
    
}



void intercalacaof1(FILE **vetorDeFitas,TipoEstatistica *estatistica){
        int fitaDeEscrita=19;
        int fitaDeLeitura=0;
    
        TipoArea area[19];
        
        int i=0,qtdBlocos=0;

        
        while(i < 19 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+i])){//preenche area inicial com o primeiro valor das fitas com blocos
            estatistica->leitura++;
            area[i].bloco=i;
            qtdBlocos++;
            i++;
        }
        
        heapTipoAreaf1(area,qtdBlocos,estatistica);

        escreverFitaSaidaf1(area,vetorDeFitas,fitaDeEscrita,estatistica);
        
        while(1){  
              
                if(!heapSaidaCheiof1(area,qtdBlocos,estatistica)){//heap é cheio quando esta no fim de todos blocos da intercalaçao atual     
                    if(leProximoItemf1(area,vetorDeFitas,fitaDeLeitura,estatistica)){
                        heapTipoAreaf1(area,qtdBlocos,estatistica);
                        escreverFitaSaidaf1(area,vetorDeFitas,fitaDeEscrita,estatistica);
                    }
                    else{//se nao ler o proximo é porque a fita acabou 
                        for(int k=1;k<qtdBlocos;k++)
                            area[k-1]=area[k];
                        qtdBlocos--;
                        heapTipoAreaf1(area,qtdBlocos,estatistica);
                        escreverFitaSaidaf1(area,vetorDeFitas,fitaDeEscrita,estatistica);
                    }
                            
                }
                else{
                    fecharFitaf1(vetorDeFitas,fitaDeEscrita,estatistica);//registro ficticio para marcar fim de bloco
                    i=0;
                    qtdBlocos=0;
                    while(i < 19 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+i])){//verifica se ainda tem blocos nas fitas de entrada
                        estatistica->leitura++;
                        area[i].bloco=i;
                        i++;
                        qtdBlocos++;
                    }  
                    if(qtdBlocos > 0){//caso tenha blocos nas fitas de entrada ele continua a intercalaçao com os novos blocos
                        heapTipoAreaf1(area,qtdBlocos,estatistica);
                        escreverFitaSaidaf1(area,vetorDeFitas,fitaDeEscrita,estatistica);
                    }
                    else{//fitas de leitura nao tem mais registros
                        qtdBlocos=0;
                        rewind(vetorDeFitas[fitaDeEscrita]);                       
                        TipoRegistro tmp;
                        while(fread(&tmp, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeEscrita])){//verifica quantos blocos tem na fita de saida
                                estatistica->leitura++;
                                estatistica->comparacoes++;
                                if(tmp.nota == 999.0)
                                    qtdBlocos++;
                            }
                        if(qtdBlocos > 1){//precisa coloca-los nas fitas de entrada e fazer a intercalaçao
                            rewind(vetorDeFitas[fitaDeEscrita]);
                            limparFitasProxf1(vetorDeFitas,fitaDeLeitura);
                            escreverFitaLeituraf1(vetorDeFitas,estatistica);
                            
                            i=0;
                            qtdBlocos=0;
                            while(i < 19 && fread(&area[i].aluno, sizeof(TipoRegistro), 1, vetorDeFitas[fitaDeLeitura+i])){//verifica quantos blocos tem nas novas fitas de entrada
                                estatistica->leitura++;
                                area[i].bloco=i;
                                qtdBlocos++;
                                i++;
                            }
                            
                            heapTipoAreaf1(area,qtdBlocos,estatistica);
                            escreverFitaSaidaf1(area,vetorDeFitas,fitaDeEscrita,estatistica); 
                            
                        }
                        else if(qtdBlocos == 1){//apenas um bloco totalmente ordenado na fita de saida
                            limparFitasProxf1(vetorDeFitas,fitaDeLeitura);
                            break;
                        }         
                    }        
            }    
        }
}
