#ifndef _REGISTRO_CABECALHO_H
#define _REGISTRO_CABECALHO_H

    /* TAD utilizado para a manipulacao do cabecalho
    *  em um arquivo binario de saida/entrada,
    *  dependendo da funcionalidade requisitada.
    */
    typedef struct cabecalho CABECALHO;

    //aloca espaco na memoria para um CABECALHO c, armazena os parametros em c e retorna o ponteiro para o mesmo
    CABECALHO *criaCabecalho(char status, long int proxByteOffSet, int nroRegArq, int nroRegRem);

    /*Definindo os setters de CABECALHO*/

    //armazena o parametro status no CABECALHO c
    void setStatusCabecalho(CABECALHO *c, char status);

    //armazena o parametro proxByteOffSet no CABECALHO c
    void setProxByteOffSetCabecalho(CABECALHO *c, long int proxByteOffSet);
    
    //armazena o parametro nroRegArq no CABECALHO c
    void setNroRegArqCabecalho(CABECALHO *c, int nroRegArq);

    //armazena o parametro nroRegRem no CABECALHO c
    void setNroRegRemCabecalho(CABECALHO *c, int nroRegRem);
    
    /*Definindo os getters de CABECALHO*/

    //retorna o status de CABECALHO c
    char getStatusCabecalho(CABECALHO *c);

    //retorna o proxByteOffSet de CABECALHO c
    long int getProxByteOffSetCabecalho(CABECALHO *c);

    //retorna o nroRegArq de CABECALHO c
    int getNroRegArqCabecalho(CABECALHO *c);

    //retorna o nroRegRem de CABECALHO c
    int getNroRegRemCabecalho(CABECALHO *c);


    //libera o espaco em memoria de *c
    void destroiCabecalho(CABECALHO **c);

#endif