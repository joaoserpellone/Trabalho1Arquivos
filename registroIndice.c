#include <stdlib.h>
#include <stdio.h>
#include "registroIndice.h"
#include "registroCabecalho.h"
#include "registroCrime.h"
#include <string.h>
#include "leituraBinario.h"

struct reg_indice_int{
    int chaveBusca;
    long int byteOffSet;
};


struct reg_indice_str{
    char chaveBusca[13];
    long int byteOffSet;
};

REG_INDICE_INT* criaRegIndiceInt(int chaveBusca, long int byteOffSet){
    REG_INDICE_INT *r = malloc(sizeof(REG_INDICE_INT));
    if(r != NULL){
        r->chaveBusca = chaveBusca;
        r->byteOffSet = byteOffSet;
    }
    return r;
}

REG_INDICE_STR* criaRegIndiceStr(char* chaveBusca, long int byteOffSet){
    REG_INDICE_STR *r = malloc(sizeof(REG_INDICE_STR));
    if(r != NULL){
        strcpy(r->chaveBusca, chaveBusca);
        r->byteOffSet = byteOffSet;
    }
    return r;
}

int getChaveBuscaRegIndiceInt(REG_INDICE_INT *r){
    if(r != NULL)
        return r->chaveBusca;
    exit(1);
}

char* getChaveBuscaRegIndiceStr(REG_INDICE_STR *r){
    if(r != NULL)
        return r->chaveBusca;
    exit(1);
}

long int getByteOffSetIndiceInt(REG_INDICE_INT *r){
    if(r != NULL)
        return r->byteOffSet;
    exit(1);
}

long int getByteOffSetIndiceStr(REG_INDICE_STR *r){
    if(r != NULL)
        return r->byteOffSet;
    exit(1);
}

void setChaveBuscaRegIndiceInt(REG_INDICE_INT *r, int chaveBusca){
    if(r != NULL)
        r->chaveBusca = chaveBusca;
}

void setChaveBuscaRegIndiceStr(REG_INDICE_STR *r, char *chaveBusca){
    if(r != NULL)
        strcpy(r->chaveBusca, chaveBusca);
}

void setByteOffSetIndiceInt(REG_INDICE_INT *r, long int byteOffSet){
    if(r != NULL)
        r->byteOffSet = byteOffSet;
}

void setByteOffSetIndiceStr(REG_INDICE_STR *r, long int byteOffSet){
    if(r != NULL)
        r->byteOffSet = byteOffSet;
}

void destroiRegIndiceInt(REG_INDICE_INT **r){
    if(r != NULL && *r != NULL){
        free(*r);
        *r = NULL;
    }
}

void destroiRegIndiceStr(REG_INDICE_STR **r){
    if(r != NULL && *r != NULL){
        free(*r);
        *r = NULL;
    }
}

void atribuiRegistroIndiceInt(REG_INDICE_INT *r1, REG_INDICE_INT *r2){
    if(r1 != NULL && r2 != NULL){
        r1->chaveBusca = r2->chaveBusca;
        r1->byteOffSet = r2->byteOffSet;    
    }
}

void atribuiRegistroIndiceStr(REG_INDICE_STR *r1, REG_INDICE_STR *r2){
    if(r1 != NULL && r2 != NULL){
        strcpy(r1->chaveBusca, r2->chaveBusca);
        r1->byteOffSet = r2->byteOffSet;    
    }
}