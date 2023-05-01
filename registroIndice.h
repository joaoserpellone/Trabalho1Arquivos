#ifndef _REG_INDICE_H
#define _REG_INDICE_H

    typedef struct reg_indice_int REG_INDICE_INT;

    typedef struct reg_indice_str REG_INDICE_STR;

    REG_INDICE_INT* criaRegIndiceInt(int chaveBusca, long int byteOffSet);

    REG_INDICE_STR* criaRegIndiceStr(char* chaveBusca, long int byteOffSet);

    int getChaveBuscaRegIndiceInt(REG_INDICE_INT *r);

    char* getChaveBuscaRegIndiceStr(REG_INDICE_STR *r);

    long int getByteOffSetIndiceInt(REG_INDICE_INT *r);

    long int getByteOffSetIndiceStr(REG_INDICE_STR *r);

    void setChaveBuscaRegIndiceInt(REG_INDICE_INT *r, int chaveBusca);

    void setChaveBuscaRegIndiceStr(REG_INDICE_STR *r, char *chaveBusca);

    void setByteOffSetIndiceInt(REG_INDICE_INT *r, long int byteOffSet);

    void setByteOffSetIndiceStr(REG_INDICE_STR *r, long int byteOffSet);

    void destroiRegIndiceInt(REG_INDICE_INT **r);

    void destroiRegIndiceStr(REG_INDICE_STR **r);

    void atribuiRegistroIndiceInt(REG_INDICE_INT *r1, REG_INDICE_INT *r2);

    void atribuiRegistroIndiceStr(REG_INDICE_STR *r1, REG_INDICE_STR *r2);

#endif