#ifndef _LEITURA_CSV_H
#define _LEITURA_CSV_H
    #include <stdio.h>
    /*
    * Funcao que le uma linha
    * de um arquivo csv, armazena
    * os dados em uma struct FICHA_CRIME
    * e retorna essa mesma struct
    */
    FICHA_CRIME *leituraCSV(FILE *arq);


#endif