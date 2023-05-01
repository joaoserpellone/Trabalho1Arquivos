#ifndef _LEITURA_BIN_H
#define _LEITURA_BIN_H

    #include <stdio.h>
    /*
    * Funcao que le o os dados
    * do cabecalho de um
    * arquivo binario e retorna
    * uma struct CABECALHO com
    * esses dados
    */
    CABECALHO *leituraCabecalho(FILE *arqBin);

    /*
    * Funcao que le um registro de
    * um arquivo binario e armazena
    * os dados em uma struct FICHA_CRIME
    * que e o retorno da funcao
    */
    FICHA_CRIME *leituraBinario(FILE *arqBin);

#endif