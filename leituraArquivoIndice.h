#ifndef _LEITURA_ARQUIVO_INDICE_H
#define _LEITURA_ARQUIVO_INDICE_H

    /*
    *   Função que lê arquivo de indice do tipo inteiro
    *   e retorna um vetor de indices lidos
    */
    REG_INDICE_INT** leituraArquivoIndiceInt(char *nomeArqIndice, int *qtdReg);

    /*
    *   Função que lê arquivo de indice do tipo string
    *   e retorna um vetor de indices lidos
    */
    REG_INDICE_STR** leituraArquivoIndiceStr(char *nomeArqIndice, int *qtdReg);

#endif