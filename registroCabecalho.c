#include "registroCabecalho.h"
#include <stdlib.h>

struct cabecalho{
    char status;
    long int proxByteOffSet;
    int nroRegArq;
    int nroRegRem;
};

CABECALHO *criaCabecalho(char status, long int proxByteOffSet, int nroRegArq, int nroRegRem){
    CABECALHO *c = (CABECALHO*) malloc(sizeof(CABECALHO));
    
    //armazena parametros caso *c foi alocado com sucesso
    if(c != NULL){
        c->status = status;
        c->proxByteOffSet = proxByteOffSet;
        c->nroRegArq = nroRegArq;
        c->nroRegRem;

        return c;
    }

    return NULL;
}

void setStatusCabecalho(CABECALHO *c, char status){
    if(c != NULL)
        c->status = status;
}

void setProxByteOffSetCabecalho(CABECALHO *c, long int proxByteOffSet){
    if(c != NULL)
        c->proxByteOffSet = proxByteOffSet;
}

void setNroRegArqCabecalho(CABECALHO *c, int nroRegArq){
    if(c != NULL)
        c->nroRegArq = nroRegArq;
}

void setNroRegRemCabecalho(CABECALHO *c, int nroRegRem){
    if(c != NULL)
        c->nroRegRem = nroRegRem;
}

char getStatusCabecalho(CABECALHO *c){
    if(c != NULL)
        return c->status;
    exit (1);
}

long int getProxByteOffSetCabecalho(CABECALHO *c){
    if(c != NULL)
        return c->proxByteOffSet;
    exit (1);
}

int getNroRegArqCabecalho(CABECALHO *c){
    if(c != NULL)
        return c->nroRegArq;
    exit (1);
}

int getNroRegRemCabecalho(CABECALHO *c){
    if(c != NULL)
        return c->nroRegRem;
    exit (1);
}

void destroiCabecalho(CABECALHO **c){
    if(c != NULL && *c != NULL){
        free(*c);
    }
}