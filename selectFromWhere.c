#include "selectFromWhere.h"
#include "registroIndice.h"
#include "registroCabecalho.h"
#include "registroCrime.h"
#include <stdio.h>
#include <stdlib.h>
#include "leituraArquivoIndice.h"
#include "leituraBinario.h"
#include <string.h>
#include <ctype.h>

void scan_quote_string(char *str) {
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
char** alocaVetorString(int m){
    char **vetor = malloc(m*sizeof(char *));

    if(vetor != NULL){
        for(int i=0; i<m; i++){
            vetor[i] = malloc(50*sizeof(char));
        }

        return vetor;
    }
}

void destroiVetorString(char **vetor, int m){
    for(int i = 0; i < m; i++)
        free(vetor[i]);
    free(vetor);
}

/*
*   Função que verifica caso o registro possua
*   o valor de campo buscado naquele registro
*   (retorna 1 caso verdadeiro)
*/
int checaCondicoes(FICHA_CRIME *fichaCrime, char *nomeCampo, char *valorCampo){
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
int verificaCampos(FICHA_CRIME *fichaCrime, int m, char **nomesCampos, char **valoresCampos, int campoEscolhido){
    //verifica se campos satisfazem a busca
    int flagAchou = 0;
    int i = 0;
    int flagPrint = 1;
    while(i < m && flagPrint == 1){
        //nao checa condicoes de busca sobre campo escolhido pois ja foi verificado
        if(i == campoEscolhido){
            i++;
            continue;
        }
        //checa condicoes para registros nao removidos
        if(getRemovido(fichaCrime) == '0')
            flagPrint = checaCondicoes(fichaCrime, nomesCampos[i], valoresCampos[i]);
        else
            flagPrint = 0;

        i++;
    }
    //caso todos as condições de busca foram satisfeitas e registro nao foi removidoS, printa registro
    if(flagPrint == 1){
        printRegistro(fichaCrime);
        flagAchou = 1;
    }

    return flagAchou;
}

/*
*   Funcao que busca linearmente registros
*   de acordo com os requisitos de busca
*/
void buscaLinear(FILE *arqBin, CABECALHO *cabecalho, int m, char **nomesCampos, char **valoresCampos){

    FICHA_CRIME *fichaCrime;
    fseek(arqBin, 0, SEEK_SET);

    //flagPrintInexistente = 1 não achou registro, flag = 0 achou registros
    int flagPrintInexistente = 1;

    //loop de busca de registros
    for(int i = 0; i < getNroRegArqCabecalho(cabecalho); i++){
        fichaCrime = leituraBinario(arqBin);

        //verifica se registro satisfaz os criteiros de busca
        int flagAchou = verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, -1);

        if(flagAchou == 1 && getRemovido(fichaCrime) != '1')
            flagPrintInexistente = 0;
        destroiFichaCrime(&fichaCrime);
    }

    if(flagPrintInexistente)
        printf("Registro inexistente.\n");
        }

void selectFromWhereInt(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n){
    //faz leitura do arquivo de indices
    int qtdReg = 0;
    REG_INDICE_INT **indices = leituraArquivoIndiceInt(nomeArqIndice, &qtdReg);
    //numero de pares de busca
    int m;
    for(int i=0; i<n; i++){
        scanf("%d", &m);
        //vetores para aramzenar os pares de busca
        char **nomesCampos = alocaVetorString(m);
        char **valoresCampos = alocaVetorString(m);

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
            scan_quote_string(valoresCampos[j]);

            //verifica se campo esta indexado
            if(strcmp(nomesCampos[j], campoIndexado) == 0){
                campoEscolhido = j;
                flag = 1;
            }
        }

        printf("Resposta para a busca %d\n", i+1);
        //busca por campo indexado
        if(flag){
            //transforma o valor do campo escolhido em int  
            int valorCampoEscolhido = atoi(valoresCampos[campoEscolhido]);
            //busca uma chave de busca igual ao valor do campo escolhido
            FICHA_CRIME *fichaCrime;

            //flagPrintInexistente = 1 não achou registro, flag = 0 achou registros
            int flagPrintInexistente = 1;
            //itera sobre as chaves de busca ate achar indice igual ao valor escolhido sobre campo
            //ou até o fim dos indices
            for(int k = 0; k < qtdReg; k++){
                if(valorCampoEscolhido == getChaveBuscaRegIndiceInt(indices[k])){
                    //calcula byteOffSet e aponta para a posição calculada
                    long int byteOffSet = getByteOffSetIndiceInt(indices[k]);
                    fseek(arqBin, byteOffSet, SEEK_SET);
                    fichaCrime = leituraBinario(arqBin);
                    
                    //verifica os campos do registro
                    int flagAchou = verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, campoEscolhido);

                    if(flagAchou == 1)
                        flagPrintInexistente = 0;

                    destroiFichaCrime(&fichaCrime);
                }
                //ja passou do indice de busca
                if(getChaveBuscaRegIndiceInt(indices[k]) > valorCampoEscolhido)
                    break;
            }

            //não achou registros que satisfazem as condições de busca
            if(flagPrintInexistente)
                printf("Registro inexistente.\n");
        }                    
        else{   //busca linear
            buscaLinear(arqBin, cabecalho, m, nomesCampos, valoresCampos);
        }

        destroiVetorString(nomesCampos, m);
        destroiVetorString(valoresCampos, m);
    } 

    //apaga vetor de indices
    for(int j = 0; j<qtdReg; j++)
        destroiRegIndiceInt(&indices[j]);
    free(indices);
}

void selectFromWhereStr(FILE *arqBin, CABECALHO *cabecalho, char *campoIndexado, char *nomeArqIndice, int n){
    int qtdReg = 0;
    //faz leitura do arquivo de indices
    REG_INDICE_STR **indices = leituraArquivoIndiceStr(nomeArqIndice, &qtdReg);
    //numero de pares de busca
    int m;
    for(int i=0; i<n; i++){
        scanf("%d", &m);
        //vetores para aramzenar os pares de busca
        char **nomesCampos = alocaVetorString(m);
        char **valoresCampos = alocaVetorString(m);

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
            scan_quote_string(valoresCampos[j]);

            //verifica se campo esta indexado
            if(strcmp(nomesCampos[j], campoIndexado) == 0){
                campoEscolhido = j;
                flag = 1;

                //trunca o campo
                valoresCampos[campoEscolhido][12] = '\0';
            }
        }
        printf("Resposta para a busca %d\n", i+1);
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
                    int flagAchou = verificaCampos(fichaCrime, m, nomesCampos, valoresCampos, campoEscolhido);

                    if(flagAchou == 1)
                        flagPrintInexistente = 0;
                        
                    destroiFichaCrime(&fichaCrime);
                }
                if(strcmp(getChaveBuscaRegIndiceStr(indices[k]), valoresCampos[campoEscolhido]) > 0)
                    break;
            }
            //não achou registros que satisfazem as condições de busca
            if(flagPrintInexistente)
                printf("Registro inexistente.\n");

        }
        else{   //busca linear
            buscaLinear(arqBin, cabecalho, m, nomesCampos, valoresCampos);
        }
    }

    //apaga vetor de indices
    for(int j = 0; j<qtdReg; j++)
        destroiRegIndiceStr(&indices[j]);
    free(indices);
}
