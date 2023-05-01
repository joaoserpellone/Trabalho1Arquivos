#include "registroCrime.h"
#include "registroCabecalho.h"
#include "leituraBinario.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//funcao que le campos de tamanho variavel ate achar delimitador '|'
void leCampoTamanhoVariavel(char *string, FILE *arqBin){
    char c = 0;

    fread(&c, sizeof(char), 1, arqBin);
    int i = 0;

    //verifica se campo nao e nulo
    if(c != '|'){
        //le cmapo ate achar delimitador
        do{
            string[i] = c;
            i++;
            fread(&c, sizeof(char), 1, arqBin);
        }while(c != '|');
        string[i] = '\0';
    }
    else{
        strcpy(string, "NULO");
    }
}

//verifica se um campo de tamanho fixo e nulo
void verificaCampoNulo(char *string){
    //caso primeiro caracter e '$' o campo e nulo
    if(string[0] == '$'){
        strcpy(string, "NULO");
    }
}

//retorna a posicao do ultimo caracter (valido) de uma string
int posicaoAposUltimoCaracter(char *string){
    int i = 0;
    while(i != 12){
        if(string[i] == '$')
            break;
        i++;
    }
    return i;
}

CABECALHO *leituraCabecalho(FILE *arqBin){
    char statusAux;
    long int proxByteOffSetAux;
    int nroRegArqAux, nroRegRemAux;
    
    fread(&statusAux, sizeof(char), 1, arqBin);
    fread(&proxByteOffSetAux, sizeof(long int), 1, arqBin);
    fread(&nroRegArqAux, sizeof(int), 1, arqBin);
    fread(&nroRegRemAux, sizeof(int), 1, arqBin);

    return criaCabecalho(statusAux, proxByteOffSetAux, nroRegArqAux, nroRegRemAux);
}

FICHA_CRIME *leituraBinario(FILE *arqBin){
    char removidoAux, dataCrimeAux[11], lugarCrimeAux[50], descricaoCrimeAux[100], marcaAux[13];
    int idAux, artigoAux;

    //leitura dos campos
    fread(&removidoAux, sizeof(char), 1, arqBin);
    fread(&idAux, sizeof(int), 1, arqBin);
    fread(&dataCrimeAux, sizeof(char), 10, arqBin); dataCrimeAux[10] = '\0';
    fread(&artigoAux, sizeof(int), 1, arqBin);
    fread(&marcaAux, sizeof(char), 12, arqBin); marcaAux[12] = '\0';
    leCampoTamanhoVariavel(lugarCrimeAux, arqBin);
    leCampoTamanhoVariavel(descricaoCrimeAux, arqBin);

    //leitura do delimtador de registro
    fgetc(arqBin);

    /*
    * tratamento de dados (campos nulos, 
    * remover '$' de campos de tamanho fixo)
    */
    verificaCampoNulo(dataCrimeAux);
    verificaCampoNulo(marcaAux);
    //coloca '\0' depois do ultimo caracter da string
    marcaAux[posicaoAposUltimoCaracter(marcaAux)] = '\0'; 

    return criaFichaCrime(removidoAux, idAux, dataCrimeAux, artigoAux, marcaAux, lugarCrimeAux, descricaoCrimeAux);
}