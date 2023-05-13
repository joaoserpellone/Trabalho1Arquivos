#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registroCrime.h"

struct fichaCrime{
	    char removido;
	    int idCrime;
	    char dataCrime[11];
	    int numeroArtigo;
	    char marcaCelular[13];
	    char lugarCrime[50];
	    char descricaoCrime[100];

};

FICHA_CRIME *criaFichaCrime(char removido, int idCrime, char *dataCrime, int numeroArtigo, char *marcaCelular, char *lugarCrime, char *descricaoCrime){
    FICHA_CRIME *f = (FICHA_CRIME*) malloc(sizeof(FICHA_CRIME));

    if(f != NULL){
        //atribui aos valores de tamanho fixo
        f->removido = removido;
        f->idCrime = idCrime;
        strcpy(f->dataCrime, dataCrime);
        f->numeroArtigo = numeroArtigo;
        strcpy(f->marcaCelular, marcaCelular);
        strcpy(f->lugarCrime, lugarCrime);
        strcpy(f->descricaoCrime, descricaoCrime);
       
    }

    return f;
}

void printRegistro(FICHA_CRIME *f){
    if(getRemovido(f) != '1'){
	    printf("%d, ", getIdCrime(f));
	    printf("%s, ",getDataCrime(f));
	    if(getNumeroArtigo(f) == -1)
	    	printf("NULO, ");
	    else printf("%d, ", getNumeroArtigo(f));
	    printf("%s, %s, %s\n", getLugarCrime(f), getDescricaoCrime(f), getMarcaCelular(f));
    }
}

char getRemovido(FICHA_CRIME *f){
    if(f != NULL){
        return f->removido;
    }
    exit(1);
}

int getIdCrime(FICHA_CRIME *f){
    if(f != NULL){
        return f->idCrime;
    }
    exit(1);
}

char *getDataCrime(FICHA_CRIME *f){
    if(f != NULL){
        return f->dataCrime;
    }
    exit(1);
}

int getNumeroArtigo(FICHA_CRIME *f){
    if(f != NULL){
        return f->numeroArtigo;
    }
    exit(1);
}

char *getMarcaCelular(FICHA_CRIME *f){
    if(f != NULL){
        return f->marcaCelular;
    }
    exit(1);
}

char *getLugarCrime(FICHA_CRIME *f){
    if(f != NULL){
        return f->lugarCrime;
    }
    exit(1);
}

char *getDescricaoCrime(FICHA_CRIME *f){
    if(f != NULL){
        return f->descricaoCrime;
    }
    exit(1);
}

void setRemovido(FICHA_CRIME *f, char removido){
    if(f != NULL){
        f->removido = removido;
    }
    exit(1);
}

void setIdCrime(FICHA_CRIME *f, int idCrime){
    if(f != NULL){
        f->idCrime = idCrime;
    }
    exit(1);
}

void setDataCrime(FICHA_CRIME *f, char *dataCrime){
    if(f != NULL){
        strcpy(f->dataCrime, dataCrime);
    }
    exit(1);
}

void setNumeroArtigo(FICHA_CRIME *f, int numeroArtigo){
    if(f != NULL){
        f->numeroArtigo = numeroArtigo;
    }
    exit(1);
}

void setMarcaCelular(FICHA_CRIME *f, char *marcaCelular){
    if(f != NULL){
        strcpy(f->marcaCelular, marcaCelular);
    }
    exit(1);
}

void setLugarCrime(FICHA_CRIME *f, char *lugarCrime){
    if(f != NULL){
        strcpy(f->lugarCrime, lugarCrime);
    }
    exit(1);
}

void setDescricaoCrime(FICHA_CRIME *f, char *descricaoCrime){
    if(f != NULL){
        strcpy(f->descricaoCrime, descricaoCrime);
    }
    exit(1);
}

void destroiFichaCrime(FICHA_CRIME **f){
    if(f != NULL && *f != NULL){
        free(*f);
    }
}

int calculaTamFichaCrime(FICHA_CRIME *f){
    /*
    * Tamanho inicialmente é
    * 1 byte (removido) + 4 bytes (id) + 
    * 10 bytes (data) + 4 bytes (artigo) +
    * 12 bytes (marca) + 3 bytes (2 delimitadores | e um fim de registro #)
    */
    int tamanho = 34;

    //calcula o tamanho das strings dos campos variaveis caso não sejam nulos
    if(strcmp(f->descricaoCrime, "NULO") != 0)
        tamanho += strlen(f->descricaoCrime);
    if(strcmp(f->lugarCrime, "NULO") != 0)
        tamanho += strlen(f->lugarCrime);
    return tamanho;
}