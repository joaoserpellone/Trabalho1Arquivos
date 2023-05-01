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
            vetor[i] = malloc(15*sizeof(char));
        }

        return vetor;
    }
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
        return !strcmp(dataCrime, valorCampo);
    }

    if(strcmp(nomeCampo, "lugarCrime") == 0){
        char *lugaCrime = getLugarCrime(fichaCrime);
        return !strcmp(lugaCrime, valorCampo);
    }

    if(strcmp(nomeCampo, "descricaoCrime") == 0){
        char *descricaoCrime = getDescricaoCrime(fichaCrime);
        return !strcmp(descricaoCrime, valorCampo);
    }
}

void buscaLinear(char *arquivoEntrada){
    //abre arquivo para leitura
    FILE *arqBin = fopen(arquivoEntrada, "rb");
    FICHA_CRIME *fichaCrime;

    //leitura de cabecalho do arquivo
    CABECALHO *cabecalho = leituraCabecalho(arqBin);
    int flagAchou = 0;
    //comeca o loop de busca
    for(int contaReg = 0; contaReg < getNroRegArqCabecalho(cabecalho); contaReg++){
        //leitura de registro
        fichaCrime = leituraBinario(arqBin);
        
        //verifica se campos satisfazem a busca
        int j = 0;
        int flagPrint = 1;
        while(flagPrint == 1 && j < m){
            flagPrint = checaCondicoes(fichaCrime, nomesCampos[j], valoresCampos[j]);
            j++;
        }
        //caso todos as condições de busca foram satisfeitas, printa registro
        if(flagPrint == 1){
            printRegistro(fichaCrime);
            flagAchou = 1;
        }
        destroiFichaCrime(&fichaCrime);
        if(feof(arqBin))
            break;
    }

    if(flagAchou == 0)
        printf("Registro inexistente.\n");

    fclose(arqBin);
}

void selectFromWhereInt(char *arquivoEntrada, char *campoIndexado, char *nomeArqIndice, int n){
    //faz leitura do arquivo de indices
    REG_INDICE_INT **indices = leituraArquivoIndiceInt(nomeArqIndice);
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
            }
        }

        printf("Resposta para a busca %d\n", i+1);
        //busca por campo indexado
        if(flag){
            //trunca o campo indexado caso necessário
                
            int valorCampoEscolhido = atoi(valoresCampos[campoEscolhido]);

            //busca uma chave de busca igual ao valor do campo escolhido
            int j = 0;
            while(valorCampoEscolhido > getChaveBuscaRegIndiceInt(indices[j])){
                j++;
            }
            
            //não achou o registro existente
            if(valorCampoEscolhido != getChaveBuscaRegIndiceInt(indices[j])){
                printf("Registro inexistente.\n");
                continue;
            }

            FILE *arqBin = fopen(arquivoEntrada, "rb");
            FICHA_CRIME *fichaCrime;

            //flagAchou = 0 nção achou registro, flag = 1 achou registros
            int flagAchou = 0;
            //itera sobre as chaves de busca enquanto é igual ao valor escolhido sobre campo
            //ou até o fim dos indices
            while(valorCampoEscolhido == getChaveBuscaRegIndiceInt(indices[j])){
                //calcula byteOffSet e aponta para a posição calculada
                long int byteOffSet = getByteOffSetIndiceInt(indices[j]);
                fseek(arqBin, byteOffSet, SEEK_SET);
                fichaCrime = leituraBinario(arqBin);

                //verifica se campos satisfazem a busca
                int k = 0;
                int flagPrint = 1;
                while(k < m && flagPrint == 1){
                    flagPrint = checaCondicoes(fichaCrime, nomesCampos[k], valoresCampos[k]);
                    k++;
                }
                //caso todos as condições de busca foram satisfeitas, printa registro
                if(flagPrint == 1){
                    printRegistro(fichaCrime);
                    flagAchou = 1;
                }
                destroiFichaCrime(&fichaCrime);

                j++;

                //chegou ao fim do vetor de indices
                if(j == (sizeof(indices)/sizeof(indices[0])))
                    break;
            }

            //não achou registros que satisfazem as condições de busca
            if(flagAchou == 0)
                printf("Registro inexistente.\n");


            //apaga vetor de indices
            for(int j = 0; j<(sizeof(indices)/sizeof(indices[0])); j++)
                destroiRegIndiceInt(&indices[i]);
            free(indices);

            fclose(arqBin);
        }                    
        else{   //busca linear
            buscaLinear(arquivoEntrada);
        }
    }  
}

void selectFromWhereStr(char *arquivoEntrada, char *campoIndexado, char *nomeArqIndice, int n){
    //faz leitura do arquivo de indices
    REG_INDICE_STR **indices = leituraArquivoIndiceInt(nomeArqIndice);
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
            }
        }
        printf("Resposta para a busca %d\n", i+1);
        //busca por campo indexado
        if(flag){
            //trunca o campo indexado caso necessário
            if(strlen(valoresCampos[campoEscolhido]) > 12)
                valoresCampos[campoEscolhido][12] = '\0';
            char valorCampoEscolhido[13];
            strcpy(valorCampoEscolhido, valoresCampos[campoEscolhido]);
            //busca uma chave de busca igual ao valor do campo escolhido
            int j = 0;
            while(strcmp(valorCampoEscolhido, getChaveBuscaRegIndiceStr(indices[j])) < 0) j++;

            //não achou o registro existente
            if(strcmp(valorCampoEscolhido, getChaveBuscaRegIndiceStr(indices[j])) != 0){
                printf("Registro inexistente.\n");
                continue;
            }
            
            FILE *arqBin = fopen(arquivoEntrada, "rb");
            FICHA_CRIME *fichaCrime;

            //flagAchou = 0 nção achou registro, flag = 1 achou registros
            int flagAchou = 0;
            //itera sobre as chaves de busca enquanto é igual ao valor escolhido sobre campo
            //ou até o fim dos indices
            while(strcmp(valorCampoEscolhido, getChaveBuscaRegIndiceStr(indices[j])) == 0 && j<sizeof(indices)/sizeof(indices[0])){
                //calcula byteOffSet e aponta para a posição calculada
                long int byteOffSet = getByteOffSetIndiceStr(indices[j]);
                fseek(arqBin, byteOffSet, SEEK_SET);
                fichaCrime = leituraBinario(arqBin);

                //verifica se campos satisfazem a busca
                int k = 0;
                int flagPrint = 1;
                while(k < m && flagPrint == 1){
                    flagPrint = checaCondicoes(fichaCrime, nomesCampos[k], valoresCampos[k]);
                    k++;
                }
                //caso todos as condições de busca foram satisfeitas, printa registro
                if(flagPrint == 1){
                    printRegistro(fichaCrime);
                    flagAchou = 1;
                }
                    
                destroiFichaCrime(&fichaCrime);
                
                j++;

                //chegou ao fim do vetor de indices
                if(j == (sizeof(indices)/sizeof(indices[0])))
                    break;
            }

            //apaga vetor de indices
            for(int j = 0; j<(sizeof(indices)/sizeof(indices[0])); j++)
                destroiRegIndiceStr(&indices[i]);
            free(indices);

            fclose(arqBin);                    
        }
        else{   //busca linear
            buscaLinear(arquivoEntrada);
        }
    }
}
