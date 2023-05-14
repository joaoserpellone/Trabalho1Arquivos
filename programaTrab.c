//Trabalho 1 da disciplina de Organização de Arquivos
//Desenvolvido por
//Daniel Afinso Borges dos Santos - 12608581
//João Pedro Oliveira Serpellone - 13671848

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "registroCrime.h"
#include "registroCabecalho.h"
#include "leituraCSV.h"
#include "leituraBinario.h"
#include "escritaBinario.h"
#include "registroIndice.h"
#include "escreveArquivoIndice.h"
#include "selectFromWhere.h"
#include "deletFromWhere.h"


void readline(char *string) {
    char c = 0;

    do {
        c = (char)getchar();
    } while (c == '\n' || c == '\r');

    int i = 0;

    do {
        string[i] = c;
        i++;
        c = getchar();
    } while (c != '\n' && c != '\r');

    string[i] = '\0';
}

void binarioNaTela(char *nomeArquivoBinario) {
    /*
     * Você não precisa entender o código dessa função.
     *
     * Use essa função para comparação no run.codes.
     * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     * Ela vai abrir de novo para leitura e depois fechar
     * (você não vai perder pontos por isso se usar ela).
     */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

int main() {

    FICHA_CRIME *fichaCrime;
    int funcionalidade;
    	
    //lê a funcionalidade desejada
	scanf("%d ", &funcionalidade);
    
	if(funcionalidade == 1){
    	
        char entrada[30];
        char saida[30];
        char cabecalhoCSV[80];
	    
        //lê os nomes dos arquivos de entrada e saída
        scanf("%s %s", entrada, saida);
        
        FILE *csv = fopen(entrada, "r");
        
        //caso o arquivo não seja aberto, imprime a mensagem de erro
        if(csv == NULL){
            printf("Falha no processamento do arquivo.");
            return 0;
        }
        
        //cria o arquivo binario e escreve o cabeçalho
        FILE *arqBin = fopen(saida, "wb");
        //le primeira linha
        fscanf(csv, "%s\n", cabecalhoCSV);
        //inicializa cabecalho
        CABECALHO *cabecalho = criaCabecalho('0', 17, 0, 0);
        escreveCabecalho(arqBin, cabecalho);
        while(!feof(csv)){
            //lê um registro por vez do csv e o escreve no arquivo binário e faz isso até achar o EOF do csv
            fichaCrime = leituraCSV(csv);
            escreveRegistro(fichaCrime, arqBin, cabecalho);
            destroiFichaCrime(&fichaCrime);
        }
		
        //atualiza status do arquivo no cabecalho para 1 
        setStatusCabecalho(cabecalho, '1');
        //sobrescreve cabecalho
        escreveCabecalho(arqBin, cabecalho);
		//libera os ponteiros usados e usa o binarioNaTela
		fclose(csv);
		fclose(arqBin);
		
		binarioNaTela(saida);
		
		destroiFichaCrime(&fichaCrime);
    	
   	}
   	
   	else if(funcionalidade == 2){
   		
   		//lê o nome do arquivo de entrada
   		char entrada[30];
   		scanf("%s", entrada);
   		
   		//abre o arquivo binario para leitura e, caso Nulo, retorna mensagem de erro
   		FILE *arq = fopen(entrada, "rb");
   		if(arq == NULL ){
   			printf("Falha no processamento do arquivo.");
   			return 0;
   		
   		}
   		
   		//lê o cabecalho e checa se o arquivo é consistente
   		CABECALHO *cabecalho = leituraCabecalho(arq);
   		if(getStatusCabecalho(cabecalho) == '0'){
   			printf("Falha no processamento do arquivo.");
   			return 0;
   		}
   		
   		//checa se o número de registros é zero ou se o numero de registros é igual ao numero de registros marcados como removidos e printa a mensagem de erro caso necessário
   		if(getNroRegArqCabecalho(cabecalho) == 0 || getNroRegArqCabecalho(cabecalho) == getNroRegRemCabecalho(cabecalho)){
   			printf("Registro inexistente.");
   			return 0;
   		}
   		
   		//lê os registros do arquivo binário e printa logo em seguida
    	for(int i = 0; i<getNroRegArqCabecalho(cabecalho); i++){
			fichaCrime = leituraBinario(arq);
            printRegistro(fichaCrime);
            destroiFichaCrime(&fichaCrime);
		}

		destroiCabecalho(&cabecalho);
		free(fichaCrime);
		fclose(arq);
		return 0;
   	
   	}
    else if(funcionalidade == 3){

        char arquivoEntrada[15];
        char campoIndexado[15];
        char tipoDado[15];
        char nomeArqIndice[15];

        scanf("%s %s %s %s", arquivoEntrada, campoIndexado, tipoDado, nomeArqIndice);

        FILE *arqBin = fopen(arquivoEntrada, "rb");

        //abre o arquivo binario para leitura e, caso Nulo, retorna mensagem de erro
        if(arqBin == NULL){
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        //lê o cabecalho e checa se o arquivo é consistente
   		CABECALHO *cabecalho = leituraCabecalho(arqBin);
   		if(getStatusCabecalho(cabecalho) == '0'){
   			printf("Falha no processamento do arquivo.");
   			return 0;
   		}
   		
   		//checa se o número de registros é zero ou se o numero de registros é igual ao numero de registros marcados como removidos e printa a mensagem de erro caso necessário
   		if(getNroRegArqCabecalho(cabecalho) == 0 || getNroRegArqCabecalho(cabecalho) == getNroRegRemCabecalho(cabecalho)){
   			printf("Registro inexistente.");
   			return 0;
   		}

        //chama função que cria arquivo de indice
        if(strcmp(tipoDado, "inteiro") == 0)
            cria_arquivo_indice_int(arqBin, cabecalho, nomeArqIndice, campoIndexado);
        else
            cria_arquivo_indice_str(arqBin, cabecalho, nomeArqIndice, campoIndexado);

        fclose(arqBin);

        binarioNaTela(nomeArqIndice);
    }
    else if(funcionalidade == 4){
        char arquivoEntrada[15];
        char campoIndexado[15];
        char tipoDado[15];
        char nomeArqIndice[15];
        int n;

        scanf("%s %s %s %s %d", arquivoEntrada, campoIndexado, tipoDado, nomeArqIndice, &n);

        FILE *arqBin = fopen(arquivoEntrada, "rb");

        //abre o arquivo binario para leitura e, caso Nulo, retorna mensagem de erro
        if(arqBin == NULL){
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        //lê o cabecalho e checa se o arquivo é consistente
   		CABECALHO *cabecalho = leituraCabecalho(arqBin);
   		if(getStatusCabecalho(cabecalho) == '0'){
   			printf("Falha no processamento do arquivo.");
   			return 0;
   		}
   		
   		//checa se o número de registros é zero ou se o numero de registros é igual ao numero de registros marcados como removidos e printa a mensagem de erro caso necessário
   		if(getNroRegArqCabecalho(cabecalho) == 0 || getNroRegArqCabecalho(cabecalho) == getNroRegRemCabecalho(cabecalho)){
   			printf("Registro inexistente.");
   			return 0;
   		}

        if(strcmp(tipoDado, "inteiro") == 0)
            selectFromWhereInt(arqBin, cabecalho, campoIndexado, nomeArqIndice, n);
        else if(strcmp(tipoDado, "string") == 0)
            selectFromWhereStr(arqBin, cabecalho, campoIndexado, nomeArqIndice, n);

        fclose(arqBin);
    }
    else if(funcionalidade == 5){
        char arquivoEntrada[15];
        char campoIndexado[15];
        char tipoDado[15];
        char nomeArqIndice[15];
        int n;

        scanf("%s %s %s %s %d", arquivoEntrada, campoIndexado, tipoDado, nomeArqIndice, &n);

        FILE *arqBin = fopen(arquivoEntrada, "rb+");

        //abre o arquivo binario para leitura e, caso Nulo, retorna mensagem de erro
        if(arqBin == NULL){
            printf("Falha no processamento do arquivo.");
            return 0;
        }

        //lê o cabecalho e checa se o arquivo é consistente
   		CABECALHO *cabecalho = leituraCabecalho(arqBin);
   		if(getStatusCabecalho(cabecalho) == '0'){
   			printf("Falha no processamento do arquivo.");
   			return 0;
   		}
   		
   		//checa se o número de registros é zero ou se o numero de registros é igual ao numero de registros marcados como removidos e printa a mensagem de erro caso necessário
   		if(getNroRegArqCabecalho(cabecalho) == 0 || getNroRegArqCabecalho(cabecalho) == getNroRegRemCabecalho(cabecalho)){
   			printf("Registro inexistente.");
   			return 0;
   		}

        if(strcmp(tipoDado, "inteiro") == 0)
            deletFromWhereInt(arqBin, cabecalho, campoIndexado, nomeArqIndice, n);
        else if(strcmp(tipoDado, "string") == 0)
            deletFromWhereStr(arqBin, cabecalho, campoIndexado, nomeArqIndice, n);

        fclose(arqBin);
        
        binarioNaTela(arquivoEntrada);
        binarioNaTela(nomeArqIndice);        
    }

    return 0;
}