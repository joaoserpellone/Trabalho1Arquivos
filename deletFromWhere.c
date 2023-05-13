#include "deletFromWhere.h"
#include "registroIndice.h"
#include "registroCabecalho.h"
#include "registroCrime.h"
#include <stdio.h>
#include <stdlib.h>
#include "leituraArquivoIndice.h"
#include "leituraBinario.h"
#include "escreveArquivoIndice.h"
#include <string.h>
#include <ctype.h>

void delet_scan_quote_string(char *str) {
    /*
     * Use essa função para ler um campo string delimitado entre aspas (").
     * Chame ela na hora que for ler tal campo. Por exemplo:
     *
     * A entrada está da seguinte forma:
     *    nomeDoCampo "MARIA DA SILVA"
     *
     * Para ler isso para as strings já alocadas str1 e str2 do seu programa,
     * você faz:
     *    scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *    scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2
     *                             // (sem as aspas)
     */

    char R;

    while ((R = getchar()) != EOF && isspace(R)) {
        ;  // ignorar espaços, \r, \n...
    }

    if (R == 'N' || R == 'n') {  // campo NULO
        getchar();
        getchar();
        getchar();  // ignorar o "ULO" de NULO.
        strcpy(str, "");  // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {  // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();  // ignorar aspas fechando
    } else if (R != EOF) {  // vc tá tentando ler uma string
                            // que não tá entre aspas!
                            // Fazer leitura normal %s então,
                            // pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else {  // EOF
        strcpy(str, "");
    }
}

/*
*   Função que aloca um vetor de strings 
*    de 15 bytes de tamanho
*/
char** delet_alocaVetorString(int m){
    char **vetor = malloc(m*sizeof(char *));

    if(vetor != NULL){
        for(int i=0; i<m; i++){
            vetor[i] = malloc(50*sizeof(char));
        }

        return vetor;
    }
}

void delet_destroiVetorString(char **vetor, int m){
    for(int i = 0; i < m; i++)
        free(vetor[i]);
    free(vetor);
}

/*
*   Função que verifica caso o registro possua
*   o valor de campo buscado naquele registro
*   (retorna 1 caso verdadeiro)
*/
int delet_checaCondicoes(FICHA_CRIME *fichaCrime, char *nomeCampo, char *valorCampo){
    if(strcmp(nomeCampo, "idCrime") == 0)
        return getIdCrime(fichaCrime) == (atoi(valorCampo));

    if(strcmp(nomeCampo, "numeroArtigo") == 0)
        return getNumeroArtigo(fichaCrime) == (atoi(valorCampo));

    if(strcmp(nomeCampo, "dataCrime") == 0){
        char *dataCrime = getDataCrime(fichaCrime);
        return strcmp(dataCrime, valorCampo) == 0;
    }

    if(strcmp(nomeCampo, "lugarCrime") == 0){
        char *lugaCrime = getLugarCrime(fichaCrime);
        return strcmp(lugaCrime, valorCampo) == 0;
    }

    if(strcmp(nomeCampo, "descricaoCrime") == 0){
        char *descricaoCrime = getDescricaoCrime(fichaCrime);
        return strcmp(descricaoCrime, valorCampo) == 0;
    }

    if(strcmp(nomeCampo, "marcaCelular") == 0){
        char *marcaCelular = getMarcaCelular(fichaCrime);
        return strcmp(marcaCelular, valorCampo) == 0;
    }
}

/*
*   Funcao que verifica se um registro
*   satisfaz a busca, comparando os valores de campos
*   passados como parametros de busca
*/
int delet_verificaCampos(FICHA_CRIME *fichaCrime, int m, char **nomesCampos, char **valoresCampos, int campoEscolhido){
    //verifica se campos satisfazem a busca
    int flagAchou = 0;
    int i = 0;
    int flagDelet = 1;
    while(i < m && flagDelet == 1){
        //nao checa condicoes de busca sobre campo escolhido pois ja foi verificado
        if(i == campoEscolhido){
            i++;
            continue;
        }
        //checa condicoes para registros nao removidos
        if(getRemovido(fichaCrime) == '0')
            flagDelet = delet_checaCondicoes(fichaCrime, nomesCampos[i], valoresCampos[i]);
        else
            flagDelet = 0;

        i++;
    }
    //caso todos as condições de busca foram satisfeitas e registro nao foi removidoS, lanca a flag para remover registro
    if(flagDelet == 1){
        flagAchou = 1;
    }

    return flagAchou;
}

/*
*   Função que busca linearmente um registro indices
*   pelo byteOffSet e eretorna sua posicao no vetor de indices
*/
int buscaIndiceRemovidoInt(REG_INDICE_INT **indices, long int byteOffSet){
    for(int i = 0; i < sizeof(indices)/sizeof(REG_INDICE_INT*); i++){
        if(getByteOffSetIndiceInt(indices[i]) == byteOffSet)
            return i;
    }

    return -1;
}

void shiftVetorIndicesInt(REG_INDICE_INT **indices, int k){
    if(k != -1){
        for(int i = k; i < sizeof(indices)/sizeof(REG_INDICE_INT*) - 1; i++){
            setChaveBuscaRegIndiceInt(indices[i], getChaveBuscaRegIndiceInt(indices[i+1]));
            setByteOffSetIndiceInt(indices[i], getByteOffSetIndiceInt(indices[i+1]));
        }
        indices = realloc(indices, sizeof(indices) - 1);
    }
}

/*
*   Funcao que remove com uma busca linear registros
*   de acordo com os requisitos de busca,
*    retorna 
*/
void removeLinearInt(FILE *arqBin, CABECALHO *cabecalho, int m,char **nomesCampos, char **valoresCampos, REG_INDICE_INT **indices, int *qtdIndicesRem){
    fseek(arqBin, 0, SEEK_SET);

    FICHA_CRIME *fichaCrime;
    long int byteOffSet = 17;

    //flagPrintInexistente = 1 não achou registro, flag = 0 achou registros
    int flagPrintInexistente = 1;

    //loop de busca de registros
    for(int i = 0; i < getNroRegArqCabecalho(cabecalho); i++){
        fichaCrime = leituraBinario(arqBin);

        //verifica se registro satisfaz os criteiros de busca
        int flagAchou = delet_verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, -1);

        //satisfez criterio de buscas
        if(flagAchou == 1 && getRemovido(fichaCrime) != '1'){
            flagPrintInexistente = 0;
            char removido = '1';
            //altera removido para '1' no arquivo de registros
            setRemovido(fichaCrime, removido);

            printRegistro(fichaCrime);

            //aponta para o comeco do registro e escreve o removido
            fseek(arqBin, byteOffSet, SEEK_SET);
            fwrite(&removido, 1, 1, arqBin);

            //busca no vetor de indices o registro com o mesmo byteOffSet e shifta o vetor
            int k = buscaIndiceRemovidoInt(indices, byteOffSet);
            shiftVetorIndicesInt(indices, k);

            //atualiza contador de removidos
            *qtdIndicesRem++;
        }
        //calcula o byteOffSet do proximo registro        
        byteOffSet += calculaTamFichaCrime(fichaCrime);
        destroiFichaCrime(&fichaCrime);
    }
        
}

/*
*   Função que atualiza a qtdRegRem do
*    cabecalho do arquivo de registros 
*   após todas as remoções feitas
*/
void atualizaCabecalhoRemovidos(FILE *arqBin, int qtdIndicesRem){

    //leitura de qtde de registros removidos do arquivo de registros
    fseek(arqBin, 13, SEEK_SET);
    int qtdRegRem;
    fread(&qtdRegRem, 4, 1, arqBin);

    //soma os registros removidos aos previamente removidos
    //e atualiza no arquivo
    qtdRegRem += qtdIndicesRem;
    fseek(arqBin, 13, SEEK_SET);
    fwrite(&qtdRegRem, 4, 1, arqBin);

}

void deletFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n){
    //qtde de registros de indices
    int qtdReg = 0;
    //qtde de registros de indices removidos
    int qtdIndicesRem = 0;

    //faz leitura do arquivo de indices
    REG_INDICE_INT **indices = leituraArquivoIndiceInt(nomeArqIndice, &qtdReg);
    //numero de pares de busca
    int m;
    for(int i=0; i<n; i++){
        scanf("%d", &m);
        //vetores para aramzenar os pares de busca
        char **nomesCampos = delet_alocaVetorString(m);
        char **valoresCampos = delet_alocaVetorString(m);

        /*  caso exista um campo que esta indexado
        *   a busca partirá desse campo
        *   caso contrário será o primeiro campo lido
        */
        int campoEscolhido = 0;
        //flag para verificar se existe um campo indexado
        int flag = 0;

        //leitura dos m pares de busca
        for(int j=0; j<m; j++){
            scanf("%s ", nomesCampos[j]);
            delet_scan_quote_string(valoresCampos[j]);

            //verifica se campo esta indexado
            if(strcmp(nomesCampos[j], campoIndexado) == 0){
                campoEscolhido = j;
                flag = 1;
            }
        }

        //busca por campo indexado
        if(flag){
            printf("Remocao indexada: ");
            //transforma o valor do campo escolhido em int  
            int valorCampoEscolhido = atoi(valoresCampos[campoEscolhido]);

            FICHA_CRIME *fichaCrime;

            //flagPrintInexistente = 1 não achou registro, flag = 0 achou registros
            int flagPrintInexistente = 1;
            //itera sobre as chaves de busca ate achar indice igual ao valor escolhido sobre campo
            //ou até o fim dos indices
            for(int k = 0; k < qtdReg - qtdIndicesRem; k++){
                if(valorCampoEscolhido == getChaveBuscaRegIndiceInt(indices[k])){
                    //calcula byteOffSet e aponta para a posição calculada
                    long int byteOffSet = getByteOffSetIndiceInt(indices[k]);
                    fseek(arqBin, byteOffSet, SEEK_SET);
                    fichaCrime = leituraBinario(arqBin);
                    
                    //verifica os campos do registro
                    int flagAchou = delet_verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, campoEscolhido);

                    if(flagAchou == 1){
                        flagPrintInexistente = 0;
                        char removido = '1';
                        //altera removido para '1' no arquivo de registros
                        fseek(arqBin, -1*calculaTamFichaCrime(fichaCrime), SEEK_CUR);
                        fwrite(&removido, 1, 1, arqBin);

                        //remove registro do vetor de indices shiftando o vetor
                        shiftVetorIndicesInt(indices, k);

                        //atualiza a contagem de registros removidos
                        qtdIndicesRem++;
                        printRegistro(fichaCrime);
                    }

                    destroiFichaCrime(&fichaCrime);
                }

                //ja passou do indice de busca
                if(getChaveBuscaRegIndiceInt(indices[k]) > valorCampoEscolhido)
                    break;
            }
            if(flagPrintInexistente)
                    printf("Num Acho\n");
        }                    
        else{   //remocao linear
            removeLinearInt(arqBin, cabecalho, m, nomesCampos, valoresCampos, indices, &qtdIndicesRem);
        }

        delet_destroiVetorString(nomesCampos, m);
        delet_destroiVetorString(valoresCampos, m);
    } 

    atualizaCabecalhoRemovidos(arqBin, qtdIndicesRem);
    escreve_arquivo_indice_int(indices, nomeArqIndice);

    //apaga vetor de indices
    for(int j = 0; j<qtdReg; j++)
        destroiRegIndiceInt(&indices[j]);
    free(indices);
}

void deletFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n){
    int qtdReg = 0;
    //faz leitura do arquivo de indices
    REG_INDICE_STR **indices = leituraArquivoIndiceStr(nomeArqIndice, &qtdReg);
    //numero de pares de busca
    int m;
    for(int i=0; i<n; i++){
        scanf("%d", &m);
        //vetores para aramzenar os pares de busca
        char **nomesCampos = delet_alocaVetorString(m);
        char **valoresCampos = delet_alocaVetorString(m);

        /*  caso exista um campo que esta indexado
        *   a busca partirá desse campo
        *   caso contrário será o primeiro campo lido
        */
        int campoEscolhido = 0;
        //flag para verificar se existe um campo indexado
        int flag = 0;

        //leitura dos m pares de busca
        for(int j=0; j<m; j++){
            scanf("%s", nomesCampos[j]);
            delet_scan_quote_string(valoresCampos[j]);

            //verifica se campo esta indexado
            if(strcmp(nomesCampos[j], campoIndexado) == 0){
                campoEscolhido = j;
                flag = 1;

                //trunca o campo
                valoresCampos[campoEscolhido][12] = '\0';
            }
        }
        //busca por campo indexado
        if(flag){
            FICHA_CRIME *fichaCrime;

            //flagPrintInexistente = 1 não achou registro, flag = 0 achou registros
            int flagPrintInexistente = 1;
            //itera sobre as chaves de busca ate achar indice igual ao valor escolhido sobre campo
            //ou até o fim dos indices
            for(int k = 0; k < qtdReg; k++){
                if(strcmp(getChaveBuscaRegIndiceStr(indices[k]), valoresCampos[campoEscolhido]) == 0){
                    //calcula byteOffSet e aponta para a posição calculada
                    long int byteOffSet = getByteOffSetIndiceStr(indices[k]);
                    fseek(arqBin, byteOffSet, SEEK_SET);
                    fichaCrime = leituraBinario(arqBin);
                    //verifica os campos do registro
                    int flagAchou = delet_verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, campoEscolhido);

                    if(flagAchou == 1)
                        flagPrintInexistente = 0;
                        
                    destroiFichaCrime(&fichaCrime);
                }
                if(strcmp(getChaveBuscaRegIndiceStr(indices[k]), valoresCampos[campoEscolhido]) > 0)
                    break;
            }

        }
        else{   //busca linear
            removeLinearInt(arqBin, cabecalho, m, nomesCampos, valoresCampos, NULL, 0);
        }
    }

    //apaga vetor de indices
    for(int j = 0; j<qtdReg; j++)
        destroiRegIndiceStr(&indices[j]);
    free(indices);
}
