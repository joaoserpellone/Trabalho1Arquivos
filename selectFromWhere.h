#ifndef _SELECT_FROM_WHERE_H
#define _SELECT_FROM_WHERE_H

    #include <stdio.h>
    #include "registroCabecalho.h"
    
    void selectFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);

    void selectFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);
#endif