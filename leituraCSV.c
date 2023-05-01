#include "registroCrime.h"
#include "leituraCSV.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 * Funcao que le determinado campo
 * de uma linha de um arquivo csv 
 * e armazena em uma string
 */
void leCampo(char *string, FILE *arq){
    char c = 0;

    int i = 0;
    c = fgetc(arq);
    //verifica se campo e vazio
    if(c != ',' && c != '\n' && c != '\r' && c != EOF){
        //le caracter por caracter ate achar fim de linha ou uma virgula
        do {
            string[i] = c;
            i++;
            c = fgetc(arq);
        } while (c != '\n' && c != '\r' && c != ',' && c != EOF);
    }

    string[i] = '\0';
}

FICHA_CRIME *leituraCSV(FILE *arq){

    //variaveis auxiliares na leitura do csv
    char idAux[5], artigoAux[4], dataAux[11], descricaoAux[100], lugarAux[50], marcaAux[13];

    //le a primeira linha do csv com dados e armazena nas variaveis auxiliares
    leCampo(idAux, arq);
    leCampo(dataAux, arq);
    leCampo(artigoAux, arq);
    leCampo(lugarAux, arq);
    leCampo(descricaoAux, arq);
    leCampo(marcaAux, arq);

    //verifica se artigo é nulo
    if(artigoAux[0] == '\0')
        return criaFichaCrime('0', atoi(idAux), dataAux, -1, marcaAux, lugarAux, descricaoAux);
    //retorna ponteiro de ficha crime
    return criaFichaCrime('0', atoi(idAux), dataAux, atoi(artigoAux), marcaAux, lugarAux, descricaoAux);
}