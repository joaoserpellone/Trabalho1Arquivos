#ifndef _ESCREVE_ARQUIVO_INDICE_H
#define _ESCREVE_ARQUIVO_INDICE_H

    /*
    * Cria um arquivo de indice
    * com os campos chave de busca inteiros e
    * o byteOffSet do registro
    * (não escreve arquivos removidos
    * ou nulos)
    */
    void cria_arquivo_indice_int(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado);

    /*
    * Escreve um arquivo de indices do
    * tipo int com base em um vetor 
    * de indices int
    */

    void escreve_arquivo_indice_int(REG_INDICE_INT **r, char *nomeArquivo);


    /*
    * Cria um arquivo de indice
    * com os campos chave de busca strings e
    * o byteOffSet do registro
    * (não escreve arquivos removidos
    * ou nulos)
    */
    void cria_arquivo_indice_str(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado);
    
#endif