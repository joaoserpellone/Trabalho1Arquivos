#ifndef _DELET_FROM_wHERE_H
#define _DELET_FROM_wHERE_H

    #include <stdio.h>
    #include "registroCabecalho.h"
    
    void deletFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);

    void deletFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n);    

#endif