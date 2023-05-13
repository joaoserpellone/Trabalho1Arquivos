#include <stdlib.h>
#include <stdio.h>
#include "registroCabecalho.h"
#include "registroCrime.h"
#include "registroIndice.h"
#include "leituraBinario.h"
#include <string.h>
#include <time.h>


/*
* -----------------------------------------
* FUNCOES PARA ESCREVER INDICES DO TIPO INT
* -----------------------------------------
*/


/*
*   Função que compara as chaves de busca de dois indices (A e B)
*   do tipo string, retorna um valor menor que zero, igaul a zero
*   e maior que zero se a chave de busca de A for menor,
*   igual e maior que a chave de B, respectivamente
*/
int comparaRegIndiceInt(REG_INDICE_INT *a, REG_INDICE_INT *b){
    int primeiro = getChaveBuscaRegIndiceInt(a); 
    int segundo = getChaveBuscaRegIndiceInt(b);
    return primeiro >= segundo;
}

/*
*   Parte do quickSort que escolhe um pivot
*   e coloca todos os elementos maiores que o pivot a sua esquerda
*   e todos os elementos menores que ele a sua direita
*/
int particao_pivot_aleatorio_int(REG_INDICE_INT **vetor, int inicio, int fim){
    int indice_pivot = (rand()%(fim-inicio))+inicio;

    /*troca de posicao o pivot escolhido com o inicio*/
    REG_INDICE_INT *aux = criaRegIndiceInt(0, 0);
    atribuiRegistroIndiceInt(aux, vetor[inicio]);
    atribuiRegistroIndiceInt(vetor[inicio], vetor[indice_pivot]);
    atribuiRegistroIndiceInt(vetor[indice_pivot], aux);
    REG_INDICE_INT *pivot = criaRegIndiceInt(0,0);
    atribuiRegistroIndiceInt(pivot, vetor[inicio]);  //guarda o pivot escolhido para comparacoes futuras

    /*inicia os indices que quando se cruzarem indicam a nova posicao do pivot*/
    int i = inicio+1;
    int j = fim;

    while(i<=j){
        while(i<=j && comparaRegIndiceInt(pivot, vetor[i]))   i++;    //move i para esquerda ate achar um elemento maior que o pivot
        while(!comparaRegIndiceInt(pivot, vetor[j]))   j--;   //move j para direita ate achar um elemento menor que o pivot
        if(i<j){        //troca de posicao, deixando o maior a direita do pivot e menor a esquerda
            atribuiRegistroIndiceInt(aux, vetor[i]);
            atribuiRegistroIndiceInt(vetor[i], vetor[j]);
            atribuiRegistroIndiceInt(vetor[j], aux);
            i++;    //anda i e j, pois na proxima iteracao ele fara duas comparacoes desnecessarias
            j--;
        }
    }

    /*j se torna a nova posicao do pivot, entao se faz necessario trocar sua posicao*/
    atribuiRegistroIndiceInt(aux, vetor[inicio]);
    atribuiRegistroIndiceInt(vetor[inicio], vetor[j]);
    atribuiRegistroIndiceInt(vetor[j], aux);

    return j;
}


/*
*   Funçaõ que ordena um vetor de
*   indices do tipo string
*/

void quickSortInt(REG_INDICE_INT **vetor, int inicio, int fim){
    if(inicio>=fim) //caso base
        return;
    
    int pivot = particao_pivot_aleatorio_int(vetor, inicio, fim);
    quickSortInt(vetor, inicio, pivot-1); //chama quick a esquerda do pivot
    quickSortInt(vetor, pivot+1, fim); //chama quick a direita do pivot
}

void cria_arquivo_indice_int(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado){

    //declaracao de variaveis
    FICHA_CRIME *fichaCrime;
    FILE *arqIndice;
    REG_INDICE_INT **indices = NULL;
    int cont = 0;
    int chaveBusca;

    long int byteOffSet = 17;       //inicializa a posição do byteOffSet após o cabecalho do arquivo de registro

    /*
    *   Lê registros e armazena num vetor de indices
    */
    for(int i =0; i < getNroRegArqCabecalho(cabecalho); i++){
        //le registro
        fichaCrime = leituraBinario(arqBin);

        //verifica qual campo é chave de busca
        if(strcmp(campoIndexado, "idCrime") == 0)
            chaveBusca = getIdCrime(fichaCrime);
        else if(strcmp(campoIndexado, "numeroArtigo") == 0)
            chaveBusca = getNumeroArtigo(fichaCrime);

        //adiciona no vetor caso não seja nulo e caso não tenha sido removido
        if(chaveBusca != -1 && (getRemovido(fichaCrime) != '1')){
            REG_INDICE_INT *aux = criaRegIndiceInt(chaveBusca, byteOffSet);
            indices = realloc(indices, (cont+1)*sizeof(REG_INDICE_INT*));
            indices[cont] = aux;
            cont++;
        }
        //calcula o próximo byteOffSet
        byteOffSet += calculaTamFichaCrime(fichaCrime);
    }
    //ordena indices
    quickSortInt(indices, 0, cont-1);

    //abre arquivo para escrita
    arqIndice = fopen(nomeArqIndice, "wb");
    //status = 0 (arquivo inconsitente)
    char status = '0';
    fwrite(&status, 1, 1, arqIndice);
    //qtdReg = cont
    fwrite(&cont, 4, 1, arqIndice);

    //escreve  os registros no arquivo de indices
    for(int i = 0; i<cont; i++){
        chaveBusca = getChaveBuscaRegIndiceInt(indices[i]);
        byteOffSet = getByteOffSetIndiceInt(indices[i]);
        fwrite(&chaveBusca, 4, 1, arqIndice);
        fwrite(&byteOffSet, 8, 1, arqIndice);
    }

    //status = 1 (arquivo consistente)
    status = '1';
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&status, 1, 1, arqIndice);

    fclose(arqIndice);
}

void escreve_arquivo_indice_int(REG_INDICE_INT **r, char *nomeArquivo){
    FILE *arqIndices = fopen(nomeArquivo, "wb");

    //escrita previa no cabecalho
    char status = '0';
    int qtdReg = 0;
    fwrite(&status, 1, 1, arqIndices);
    fwrite(&qtdReg, 4, 1, arqIndices);

    //escrita dos registros no arquivo
    for(int i = 0; i < sizeof(r)/sizeof(REG_INDICE_INT*); i++){
        qtdReg++;
        int chaveBusca = getChaveBuscaRegIndiceInt(r[i]);
        long int byteOffSet = getByteOffSetIndiceInt(r[i]);
        fwrite(&chaveBusca, 4, 1, arqIndices);
        fwrite(&byteOffSet, 8, 1, arqIndices);
    }

    qtdReg++;
    //atualiza cabecalho e fecha arquivo
    fseek(arqIndices, 0, SEEK_SET);
    fwrite(&qtdReg, 4, 1, arqIndices);

    fclose(arqIndices);
}

/*
* -----------------------------------------
* FUNCOES PARA ESCREVER INDICES DO TIPO STR
* -----------------------------------------
*/

/*
*   Função que compara as chaves de busca de dois indices (A e B)
*   do tipo string, retorna um valor menor que zero, igaul a zero
*   e maior que zero se a chave de busca de A for menor,
*   igual e maior que a chave de B, respectivamente
*/
int comparaRegIndiceStr(REG_INDICE_STR *a, REG_INDICE_STR *b){
    char primeiro[13]; 
    char segundo[13];
    strcpy(primeiro, getChaveBuscaRegIndiceStr(a));
    strcpy(segundo, getChaveBuscaRegIndiceStr(b));
    return strcmp(primeiro, segundo);
}


/*
*   Parte do quickSort que escolhe um pivot
*   e coloca todos os elementos maiores que o pivot a sua esquerda
*   e todos os elementos menores que ele a sua direita
*/

int particao_pivot_aleatorio_str(REG_INDICE_STR **vetor, int inicio, int fim){
    int indice_pivot = (rand()%(fim-inicio))+inicio;

    /*troca de posicao o pivot escolhido com o inicio*/
    REG_INDICE_STR *aux = criaRegIndiceStr("", 0);
    atribuiRegistroIndiceStr(aux, vetor[inicio]);
    atribuiRegistroIndiceStr(vetor[inicio], vetor[indice_pivot]);
    atribuiRegistroIndiceStr(vetor[indice_pivot], aux);
    REG_INDICE_STR *pivot = criaRegIndiceStr("",0);
    atribuiRegistroIndiceStr(pivot, vetor[inicio]);  //guarda o pivot escolhido para comparacoes futuras

    /*inicia os indices que quando se cruzarem indicam a nova posicao do pivot*/
    int i = inicio+1;
    int j = fim;

    while(i<=j){
        while(i<=j && (comparaRegIndiceStr(pivot, vetor[i]) >= 0))   i++;    //move i para esquerda ate achar um elemento maior que o pivot
        while(comparaRegIndiceStr(pivot, vetor[j]) < 0)   j--;   //move j para direita ate achar um elemento menor que o pivot
        if(i<j){        //troca de posicao, deixando o maior a direita do pivot e menor a esquerda
            atribuiRegistroIndiceStr(aux, vetor[i]);
            atribuiRegistroIndiceStr(vetor[i], vetor[j]);
            atribuiRegistroIndiceStr(vetor[j], aux);
            i++;    //anda i e j, pois na proxima iteracao ele fara duas comparacoes desnecessarias
            j--;
        }
    }

    /*j se torna a nova posicao do pivot, entao se faz necessario trocar sua posicao*/
    atribuiRegistroIndiceStr(aux, vetor[inicio]);
    atribuiRegistroIndiceStr(vetor[inicio], vetor[j]);
    atribuiRegistroIndiceStr(vetor[j], aux);

    return j;
}

/*
*   Funçaõ que ordena um vetor de
*   indices do tipo string
*/
void quickSortStr(REG_INDICE_STR **vetor, int inicio, int fim){
    if(inicio>=fim) //caso base
        return;
    
    int pivot = particao_pivot_aleatorio_str(vetor, inicio, fim);
    quickSortStr(vetor, inicio, pivot-1); //chama quick a esquerda do pivot
    quickSortStr(vetor, pivot+1, fim); //chama quick a direita do pivot
}

/*
*   Função que trata possíveis inconsistencias
*   na string de chave de busca
*/
void trataChaveBusca(char *chaveBusca){
    /*
    *   caso a string da chave de busca seja maior do que possa ser
    *   armazenado no arquivo, a chave é truncada
    *   caso contrário preenche os espaços vazios com $
    */
    if(strlen(chaveBusca) > 12){
        chaveBusca[12] = '\0';
    }
    else{
        int i = strlen(chaveBusca);
        while(i != 12){
            chaveBusca[i] = '$';
            i++;
        }
        chaveBusca[i] = '\0';
    }
}

void cria_arquivo_indice_str(FILE *arqBin, CABECALHO *cabecalho, char *nomeArqIndice, char *campoIndexado){

    //declaracao de variaveis
    FICHA_CRIME *fichaCrime;
    FILE *arqIndice;
    REG_INDICE_STR **indices = NULL;
    int cont = 0;
    char chaveBusca[100];

    long int byteOffSet = 17;       //inicializa a posição do byteOffSet após o cabecalho do arquivo de registro

    /*
    *   Lê registros e armazena num vetor de indices
    */
    for(int i =0; i < getNroRegArqCabecalho(cabecalho); i++){
        //le registro
        fichaCrime = leituraBinario(arqBin);

        //verifica qual campo é chave de busca
        if(strcmp(campoIndexado, "descricaoCrime") == 0)
            strcpy(chaveBusca, getDescricaoCrime(fichaCrime));
        else if(strcmp(campoIndexado, "lugarCrime") == 0)
            strcpy(chaveBusca, getLugarCrime(fichaCrime));
        else if(strcmp(campoIndexado, "dataCrime") == 0)
            strcpy(chaveBusca, getDataCrime(fichaCrime));

        //adiciona no vetor caso não seja nulo e caso não tenha sido removido
        if((chaveBusca[0] != '$') && (strcmp(chaveBusca, "NULO") != 0) && (getRemovido(fichaCrime) != '1')){
            //trata inconsistencias na chave de busca
            trataChaveBusca(chaveBusca);
            REG_INDICE_STR *aux = criaRegIndiceStr(chaveBusca, byteOffSet);
            indices = realloc(indices, (cont+1)*sizeof(REG_INDICE_STR*));
            indices[cont] = aux;
            cont++;
        }
        //calcula o próximo byteOffSet
        byteOffSet += calculaTamFichaCrime(fichaCrime);
    }
    //ordena indices
    quickSortStr(indices, 0, cont-1);
    
    //abre arquivo para escrita
    arqIndice = fopen(nomeArqIndice, "wb");
    //status = 0 (arquivo inconsitente)
    char status = '0';
    fwrite(&status, 1, 1, arqIndice);
    //qtdReg = cont
    fwrite(&cont, 4, 1, arqIndice);

    //escreve  os registros no arquivo de indices
    for(int i = 0; i<cont; i++){
        strcpy(chaveBusca, getChaveBuscaRegIndiceStr(indices[i]));
        //preenche espacos vazios
        byteOffSet = getByteOffSetIndiceStr(indices[i]);
        fwrite(&chaveBusca, 12, 1, arqIndice);
        fwrite(&byteOffSet, 8, 1, arqIndice);
    }

    //status = 1 (arquivo consistente)
    status = '1';
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&status, 1, 1, arqIndice);

    fclose(arqIndice);
}