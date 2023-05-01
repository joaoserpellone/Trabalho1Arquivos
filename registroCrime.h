#ifndef _REGISTROCRIME_H
#define _REGISTROCRIME_H

    /* TAD para manipulacao tanto de dados
    *  vindo de um .csv, quanto de um .bin 
    */
	
    typedef struct fichaCrime FICHA_CRIME;

    //cria uma nova ficha crime e ja registra todos os parametros
    FICHA_CRIME *criaFichaCrime(char removido, int idCrime, char *dataCrime, int numeroArtigo, char *marcaCelular, char *lugarCrime, char *descricaoCrime);

    void printRegistro(FICHA_CRIME *f);

    /*definindo os getters*/

    //retorna removido de FICHA_CRIME f
    char getRemovido(FICHA_CRIME *f);

    //retorna idCrime de FICHA_CRIME f
    int getIdCrime(FICHA_CRIME *f);

    //retorna dataCrime de FICHA_CRIME f
    char *getDataCrime(FICHA_CRIME *f);

    //retorna numeroArtigo de FICHA_CRIME f
    int getNumeroArtigo(FICHA_CRIME *f);

    //retorna marcaCelular de FICHA_CRIME f
    char *getMarcaCelular(FICHA_CRIME *f);

    //retorna lugarCrime de FICHA_CRIME f
    char *getLugarCrime(FICHA_CRIME *f);

    //retorna descricaoCrime de FICHA_CRIME f
    char *getDescricaoCrime(FICHA_CRIME *f);

    //libera o espaco em memoria de *f
    void destroiFichaCrime(FICHA_CRIME **f);

    //calcula o númerio de bytes que uma FICHA_CRIME ocupa em um arquivo binário
    int calculaTamFichaCrime(FICHA_CRIME *f);

#endif
