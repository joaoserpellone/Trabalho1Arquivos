#ifndef _SELECT_FROM_WHERE_H
#define _SELECT_FROM_WHERE_H

    #include <stdio.h>
    #include "registroCabecalho.h"

    /*
    *   Funcao que realiza sequencia de m buscas
    *   de registros, a busca pode ser indexada (tipo int) 
    *   e com mais campos ou linear, 
    *   diretamente no arquivo binario 
    */
    void selectFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);


    /*
    *   Funcao que realiza sequencia de m buscas
    *   de registros, a busca pode ser indexada (tipo string) 
    *   e com mais campos ou linear, 
    *   diretamente no arquivo binario 
    */
    void selectFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);
#endif