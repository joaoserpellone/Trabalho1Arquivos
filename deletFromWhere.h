#ifndef _DELET_FROM_wHERE_H
#define _DELET_FROM_wHERE_H

    #include <stdio.h>
    #include "registroCabecalho.h"
    
    /*
    *   Funcao que recebe sequencia de campos a serem
    *   buscados para remocao de registro, eh usada para
    *   remocao de indices (caso tenha) do tipo int
    */
    void deletFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);


    /*
    *   Funcao que recebe sequencia de campos a serem
    *   buscados para remocao de registro, eh usada para
    *   remocao de indices (caso tenha) do tipo string
    */
    void deletFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);    

#endif