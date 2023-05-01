#include "registroCrime.h"
#include "registroCabecalho.h"
#include "leituraBinario.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "escritaBinario.h"

void escreveMarcaCelular(FICHA_CRIME* registro, FILE *arqBin){
	//trata o marcaCelular para que todos os espaços vazios sejam preenchidos com '$' e ja considera o caso de marca nula
	char* marcaAux = getMarcaCelular(registro);
	int contaCaracter = 0;
	char marca[13];
	
	while(marcaAux[contaCaracter] != '\0'){
		
		marca[contaCaracter] = marcaAux[contaCaracter];
		contaCaracter++;
		
	}
	
	for(contaCaracter; contaCaracter < 12; contaCaracter++){
	
		marca[contaCaracter] = '$';
	
	}

	marca[contaCaracter] = '\0';
	
	//escreve marcaCelular no registro
	
	fwrite(marca, 1, 12, arqBin);
}

int escreveCampoVariavel(char *string, FILE *arqBin){
	int contaCaracter = 0;
		
	while(string[contaCaracter] != '\0'){
			
		contaCaracter++;
			
	}
	string[contaCaracter] = '|';
	contaCaracter++;
	//escreve string no registro
	fwrite(string, 1, contaCaracter, arqBin);

	return contaCaracter;
}

void escreveCabecalho(FILE *arqBin, CABECALHO *cabecalho)
{
	//ponteiro do arquivo aponta para início do arquivo
    fseek(arqBin, 0, SEEK_SET);
	char status = getStatusCabecalho(cabecalho);
	long int proxByteOffSet = getProxByteOffSetCabecalho(cabecalho);
	int numRegArq = getNroRegArqCabecalho(cabecalho);
	int numRegRem = getNroRegRemCabecalho(cabecalho);
	fwrite(&status, 1, 1, arqBin);
	fwrite(&proxByteOffSet, 8, 1, arqBin);
	fwrite(&numRegArq, 4, 1, arqBin);
	fwrite(&numRegRem, 4, 1, arqBin);
}

void escreveRegistro(FICHA_CRIME* registro, FILE* arqBin, CABECALHO *cabecalho){
	//inicializa o tamanho do registro, já contando os campos fixos
	//depois atualiza com os campos variaveis
	int tamReg = 31;
	//escreve o valor de removido do registro
	char removido = getRemovido(registro);
	fwrite(&removido, 1, 1, arqBin);
	//incrementa o numero de registros removidos caso necessario
	if(getRemovido(registro) == 1)
		setNroRegRemCabecalho(cabecalho, getNroRegRemCabecalho(cabecalho) + 1);
	
	//escreve idCrime do registro
	int id = getIdCrime(registro);
	fwrite(&id, 4, 1, arqBin);
	
	//escreve dataCrime do registro
	//checa se a data é nula
	char* dataAux = getDataCrime(registro);
	if(dataAux[0] == '\0'){
		//preenche a string com '$' no caso de data nula
		for(int i=0; i<10; i++){
		
			dataAux[i] = '$';
		
		}
	}
	fwrite(dataAux, 1, 10, arqBin);
	
	//escreve numeroArtigo do registro
	int numArtigo = getNumeroArtigo(registro);
	fwrite(&numArtigo, 4, 1, arqBin);

	escreveMarcaCelular(registro, arqBin);

	char* lugar = getLugarCrime(registro);
	tamReg += escreveCampoVariavel(lugar, arqBin);
	
	//acha o fim da string para inserir o delimitador de campo e, caso nula, apenas escreve o delimitador
	char* descricao = getDescricaoCrime(registro);
	tamReg += escreveCampoVariavel(descricao, arqBin);
	
	//escreve o delimitador de registro
	char delimitadorReg = '#';
	fwrite(&delimitadorReg, 1, 1, arqBin);
	tamReg++;
	//incrementa numReg
	setNroRegArqCabecalho(cabecalho, getNroRegArqCabecalho(cabecalho) + 1);

	//atualiza o proxByteOffSet
	setProxByteOffSetCabecalho(cabecalho, getProxByteOffSetCabecalho(cabecalho) + tamReg);
}