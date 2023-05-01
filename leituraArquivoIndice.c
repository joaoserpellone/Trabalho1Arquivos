#include <stdlib.h>
#include <stdio.h>
#include "registroIndice.h"
#include "leituraArquivoIndice.h"

REG_INDICE_INT** leituraArquivoIndiceInt(char *nomeArqIndice){
    FILE *arqIndice = fopen(nomeArqIndice, "rb");
    //leitura do cabecalho do arquivo
    fgetc(arqIndice);

    //declaração de variaveis
    REG_INDICE_INT **vetor = NULL;
    int cont = 0;
    int chaveBusca;
    long int byteOffSet;
    //loop de leitura
    while(!feof(arqIndice)){
        //realoca vetor
        vetor = realloc(vetor, (cont + 1)*sizeof(REG_INDICE_INT*));

        //leitura do arqIndice e adiciona ao vetor
        fread(&chaveBusca, 4, 1, arqIndice);
        fread(&byteOffSet, 8, 1, arqIndice);
        vetor[cont] = criaRegIndiceInt(chaveBusca, byteOffSet);
        //incrementa contador
        cont++;
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

REG_INDICE_STR** leituraArquivoIndiceStr(char *nomeArqIndice){
    FILE *arqIndice = fopen(nomeArqIndice, "rb");
    //leitura do cabecalho do arquivo
    //declaração de variaveis
    REG_INDICE_STR **vetor = NULL;
    int cont = 0;
    char chaveBusca[13];
    long int byteOffSet;
    //loop de leitura
    while(!feof(arqIndice)){
        //realoca vetor
        vetor = realloc(vetor, (cont + 1)*sizeof(REG_INDICE_STR*));

        //leitura do arqIndice e adiciona ao vetor
        fread(&chaveBusca, 12, 1, arqIndice);
        fread(&byteOffSet, 8, 1, arqIndice);
        //finaliza string com \0
        chaveBusca[12] = '\0';

        eliminaSifrao(chaveBusca);
        vetor[cont] = criaRegIndiceStr(chaveBusca, byteOffSet);

        //incrementa contador
        cont++;
    }
    fclose(arqIndice);
    return vetor;
}