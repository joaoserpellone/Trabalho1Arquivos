#ifndef _ESCRITA_BIN_H
#define _ESCRITA_BIN_H
	#include <stdio.h>

	/*
	* Funcao que escreve o 
	* cabecalho do arquivo binario
	* de acordo com o que esta em ponteiro para 
	* registro de cabecalho
	*/
	void escreveCabecalho(FILE* arqBin, CABECALHO *cabecalho);
	
	/*
	* Funcao que escreve um
	* registro no arquivo binario
	* partindo de uma struct dada
	* Além disso calcula o proxByteOffSet,
	* regRem e regArq do cabecalho, durante
	* sua execução.
	*/
	void escreveRegistro(FICHA_CRIME* registro, FILE* arqBin, CABECALHO *cabecalho);
	
#endif
