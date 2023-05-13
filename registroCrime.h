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

    //atribui o parametro removido ao campo removido de FICHA_CRIME f 
    void setRemovido(FICHA_CRIME *f, char removido);

    //atribui o parametro idCrime ao campo idCrime de FICHA_CRIME f 
    void setIdCrime(FICHA_CRIME *f, int idCrime);
    
    //atribui o parametro dataCrime ao campo dataCrime de FICHA_CRIME f
    void setDataCrime(FICHA_CRIME *f, char *dataCrime);

    //atribui o parametro numeroArtigo ao campo numeroArtigo de FICHA_CRIME f
    void setNumeroArtigo(FICHA_CRIME *f, int numeroArtigo);

    //atribui o parametro marcaCelular ao campo marcaCelular de FICHA_CRIME f
    void setMarcaCelular(FICHA_CRIME *f, char *marcaCelular);

    //atribui o parametro lugarCrime ao campo lugarCrime de FICHA_CRIME f
    void setLugarCrime(FICHA_CRIME *f, char *lugarCrime);

    //atribui o parametro descricaoCrime ao campo descricaoCrime de FICHA_CRIME f
    void setDescricaoCrime(FICHA_CRIME *f, char *descricaoCrime);

    //libera o espaco em memoria de *f
    void destroiFichaCrime(FICHA_CRIME **f);

    //calcula o númerio de bytes que uma FICHA_CRIME ocupa em um arquivo binário
    int calculaTamFichaCrime(FICHA_CRIME *f);

#endif
