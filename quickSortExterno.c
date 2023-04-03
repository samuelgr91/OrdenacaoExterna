#include "quickSortExterno.h"

void QuickSortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, ests *estatisticas)
{
    int i, j;
    TipoAreaQuick *Area = (TipoAreaQuick *)malloc(TAMAREA * sizeof(TipoAreaQuick));
    if (Dir - Esq < 1)
    {
        free(Area);
        return;
    }
    FAVazia(Area);
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, estatisticas);

    fflush(*ArqLi);
    fflush(*ArqEi);
    fflush(*ArqLEs);

    if (i - Esq < Dir - j)
    {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, estatisticas);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, estatisticas);
    }
    else
    {
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, estatisticas);
        QuickSortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, estatisticas);
    }

    free(Area);
}

void FAVazia(TipoAreaQuick *Area)
{
    for (int i = 0; i < TAMAREA; i++)
    {
        Area->aluno[i].matricula = -1;
        Area->aluno[i].nota = -1;
        strcpy(Area->aluno[i].estado, "LIXO");
        strcpy(Area->aluno[i].cidade, "LIXO");
        strcpy(Area->aluno[i].curso, "LIXO");
    }
}

int ObterNumCelOcupadas(TipoAreaQuick *Area, ests *estatisticas)
{
    int cont = 0;
    incComparacao(estatisticas);
    while (Area->aluno[cont].nota != -1 && cont < TAMAREA)
    {
        incComparacao(estatisticas);
        cont++;
    }
    return cont;
}

void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer, ests *estatisticas)
{
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    incTransferenciaLeitura(estatisticas);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    *OndeLer = FALSE;
}

void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer, ests *estatisticas)
{
    incTransferenciaLeitura(estatisticas);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    *OndeLer = TRUE;
}

void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es, ests *estatisticas)
{
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    incTransferenciaEscrita(estatisticas);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei, ests *estatisticas)
{
    incTransferenciaEscrita(estatisticas);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
}

void RetiraMax(TipoAreaQuick *Area, TipoRegistro *R, int *NRArea, ests *estatisticas)
{
    RetiraUltimo(Area, R, estatisticas);
    *NRArea = ObterNumCelOcupadas(Area, estatisticas);
}

void RetiraMin(TipoAreaQuick *Area, TipoRegistro *R, int *NRArea, ests *estatisticas)
{
    RetiraPrimeiro(Area, R, estatisticas);
    *NRArea = ObterNumCelOcupadas(Area, estatisticas);
}

void RetiraUltimo(TipoAreaQuick *Area, TipoRegistro *R, ests *estatisticas)
{
    int cont = 0;
    incComparacao(estatisticas);
    while (Area->aluno[cont].nota != -1 && cont < TAMAREA)
    {
        incComparacao(estatisticas);
        cont++;
    }

    cont--;
    *R = Area->aluno[cont];

    Area->aluno[cont].matricula = -1;
    Area->aluno[cont].nota = -1.0;
    strcpy(Area->aluno[cont].estado, "LIxo");
    strcpy(Area->aluno[cont].cidade, "LIXO");
    strcpy(Area->aluno[cont].curso, "LIXO");
}

void RetiraPrimeiro(TipoAreaQuick *Area, TipoRegistro *R, ests *estatisticas)
{
    int i;
    int j = 1;

    // faço o retorno por referencia
    *R = Area->aluno[0];
    incComparacao(estatisticas);
    // percorro a área a partir do segundo elemento
      while (Area->aluno[j].nota != -1 && i < TAMAREA - 1)
    {
        incComparacao(estatisticas);
        Area[i] = Area[j];
        i++;
        j++;
    }

    // completo com lixo no ultimo elemento
    Area->aluno[TAMAREA - 1].matricula = -1;
    Area->aluno[TAMAREA - 1].nota = -1.0;
    strcpy(Area->aluno[TAMAREA - 1].estado, "LIXO");
    strcpy(Area->aluno[TAMAREA - 1].cidade, "LIXO");
    strcpy(Area->aluno[TAMAREA - 1].curso, "LIXO");
}

void InsereItem(TipoRegistro UltLido, TipoAreaQuick *Area, ests *estatisticas)
{
    int i = 0; // percorre
    int j = 8; // antepenultimo item da area
    int k = 9; // penultimo item da área
    incComparacao(estatisticas);
    incComparacao(estatisticas);
    while (Area->aluno[i].nota < UltLido.nota && Area->aluno[i].matricula != -1 && i < TAMAREA)
    {
        incComparacao(estatisticas);
        incComparacao(estatisticas);
        i++;
    }

    while (j >= i)
    {
        // reorganizo a área de acordo com o índice no vetor
        Area[k] = Area[j];
        j--;
        k--;
    }
    // faço a inserção no indice
    Area->aluno[i] = UltLido;
}

void InserirArea(TipoAreaQuick *Area, TipoRegistro *UltLido, int *NRArea, ests *estatisticas)
{
    // Insere UltLido de forma ordenada na Area
    InsereItem(*UltLido, Area, estatisticas);
    *NRArea = ObterNumCelOcupadas(Area, estatisticas);
}

// Funçção de Partição do QuickSort Externo
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoAreaQuick *Area, int Esq, int Dir, int *i, int *j, ests *estatisticas)
{
    int Ls = Dir;
    int Es = Dir;
    int Li = Esq;
    int Ei = Esq;
    int NRArea = 0;
    double Linf = INT_MIN;
    double Lsup = INT_MAX;

    short OndeLer = TRUE;

    TipoRegistro R;

    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(TipoRegistro), SEEK_SET);

    *i = Esq - 1;
    *j = Dir + 1;

    while (Ls >= Li)
    {
        TipoRegistro UltLido;

        if (NRArea < TAMAREA - 1)
        {
            if (OndeLer)
            {
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatisticas);
            }
            else
            {
                LeInf(ArqLi, &UltLido, &Li, &OndeLer, estatisticas);
            }

            InserirArea(Area, &UltLido, &NRArea, estatisticas);
            continue;
        }

        if (Ls == Es)
        {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatisticas);
        }
        else if (Li == Ei)
        {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, estatisticas);
        }
        else if (OndeLer)
        {
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatisticas);
        }
        else
        {
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, estatisticas);
        }

        incComparacao(estatisticas);

        if (UltLido.nota > Lsup)
        {
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es, estatisticas);
            continue;
        }

        if (UltLido.nota < Linf)
        {
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei, estatisticas);
            continue;
        }

        InserirArea(Area, &UltLido, &NRArea, estatisticas);

        if (Ei - Esq < Dir - Es) // Subarquivo 1 (Ei - Esq) e subarquivo 2 (Dir - Es)
        {
            RetiraMin(Area, &R, &NRArea, estatisticas);
            EscreveMin(ArqEi, R, &Ei, estatisticas);
            Linf = R.nota;
        }
        else
        {
            RetiraMax(Area, &R, &NRArea, estatisticas);
            EscreveMax(ArqLEs, R, &Es, estatisticas);
            Lsup = R.nota;
        }
    }

    while (Ei <= Es)
    {
        RetiraMin(Area, &R, &NRArea, estatisticas);
        EscreveMin(ArqEi, R, &Ei, estatisticas);
    }
}

void imprimirArea(TipoAreaQuick *Area)
{
    printf("===== Área =====\n");
    for (int i = 0; i < TAMAREA; i++)
    {
        imprimeAlunoQuick(Area[i].aluno);
    }
    printf("\n");
}

void imprimeAlunoQuick(TipoRegistro *aluno)
{
    printf("Matricula: %ld | Nota: %.2f | Estado: %s | Cidade: %s | Curso: %s \n", aluno->matricula, aluno->nota, aluno->estado, aluno->cidade, aluno->curso);
}