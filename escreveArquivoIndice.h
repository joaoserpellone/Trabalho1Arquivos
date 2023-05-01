#ifndef _ESCREVE_ARQUIVO_INDICE
#define _ESCREVE_ARQUIVO_INDICE

    /*
    * Cria um arquivo de indice
    * com os campos chave de busca inteiros e
    * o byteOffSet do registro
    * (não escreve arquivos removidos
    * ou nulos)
    */
    void cria_arquivo_indice_int(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado);

    /*
    * Cria um arquivo de indice
    * com os campos chave de busca strings e
    * o byteOffSet do registro
    * (não escreve arquivos removidos
    * ou nulos)
    */
    void cria_arquivo_indice_str(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado);
    
#endif