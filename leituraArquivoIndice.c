#include <stdlib.h>
#include <stdio.h>
#include "registroIndice.h"
#include "leituraArquivoIndice.h"

REG_INDICE_INT** leituraArquivoIndiceInt(char *nomeArqIndice, int *qtdReg){
    FILE *arqIndice = fopen(nomeArqIndice, "rb");
    //leitura do cabecalho do arquivo
    fgetc(arqIndice);
    //leitura da quantidade de registro de indices
    fread(qtdReg, 4, 1, arqIndice);
    //declaração de variaveis e aslocacao de vetor de indices
    REG_INDICE_INT **vetor = malloc((*qtdReg)*sizeof(REG_INDICE_INT*));
    int chaveBusca;
    long int byteOffSet;
    //loop de leitura
    for(int i = 0; i < *qtdReg; i++){

        //leitura do arqIndice e adiciona ao vetor
        fread(&chaveBusca, 4, 1, arqIndice);
        fread(&byteOffSet, 8, 1, arqIndice);
        vetor[i] = criaRegIndiceInt(chaveBusca, byteOffSet);

    }

    fclose(arqIndice);
    return vetor;
}

/*
*   Função qu elimina $ da string que
*   que foi lida no arquivo de indice
*/
void eliminaSifrao(char *str){
    int i=0;
    while(str[i] != '$' && i < 12)
        i++;
    str[i] = '\0';
}

REG_INDICE_STR** leituraArquivoIndiceStr(char *nomeArqIndice, int *qtdReg){
    FILE *arqIndice = fopen(nomeArqIndice, "rb");
    //leitura do cabecalho do arquivo
    fgetc(arqIndice);
    //leitura da quantidade de registro de indices
    fread(qtdReg, 4, 1, arqIndice);
    //declaração de variaveis e aslocacao de vetor de indices
    REG_INDICE_STR **vetor = malloc((*qtdReg)*sizeof(REG_INDICE_STR*));
    char chaveBusca[13];
    long int byteOffSet;
    //loop de leitura
    for(int i = 0; i < *qtdReg; i++){
        //leitura do arqIndice e adiciona ao vetor
        fread(&chaveBusca, 12, 1, arqIndice);
        fread(&byteOffSet, 8, 1, arqIndice);
        //finaliza string com \0
        chaveBusca[12] = '\0';

        eliminaSifrao(chaveBusca);
        vetor[i] = criaRegIndiceStr(chaveBusca, byteOffSet);

    }
    fclose(arqIndice);
    return vetor;
}