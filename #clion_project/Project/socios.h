
#define CONFIG_FILE "config.txt"

#ifndef PROJETO_MONTANHISMO_SOCIOS_H
#define PROJETO_MONTANHISMO_SOCIOS_H

#define MAXCHAR_NOMESOCIO 100
#define MAX_SOCIOS 100
#define QUOTA_ANUAL 50.0

#define BUFFER_SIZE 1024

// Estrutura para as Informações de um sócio.
typedef struct SOCIO {

    int codigo;
    char nome[MAXCHAR_NOMESOCIO];
    double quota;

} Socio;

typedef struct TNODE {
    Socio socio;
    int disponivel;
    int left;
    int right;
} Tnode;

//Utilizadas para as mensagens de erros.
char error_message[BUFFER_SIZE];
int error_code;

//Inicializa a matriz.
void init();

//Cria o socio.
Socio criar_socio(int, char*);

//Coloca o socio na matriz.
//Devolve 1 se foi colocado com sucesso e o parametro é o socio.
int colocar(Socio);

//Árvore utilizada para os socios.
//Devolve a posição na matriz. Parametros: 1º: posição da matriz, 2ª: posição do novo nó na matriz.
int addTree(int, int);

//Árvore utilizada para as quotas.
//Parametros: socio e quota.
int inserir_quota(int, double);

//Função utilizada para testes (permite fazer o print da árvore construida).
//void print();

//Permitirá fazer o print da árvore com o nome do socio e a respetiva quota.
//Parametros: no da matriz.
void printTree(int);

//Permite obter o nome dos socios definidos no ficheiro "socios.txt" e as quotas dos mesmos do ficheiro "quotas.txt".
//Erros que poderão ocorrer ao não ser cumprida a formatação definida nos ficheiros.
//Devolve 1 se correu bem.
int loadSocios(FILE*);
int loadQuotas(FILE*);

//Funções que permitem fazer a listagem dos socios com as respetivas quotas (criará os outputs pretendidos).
void listar(FILE*, FILE*);
void listarSociosSemDividas(FILE*);
void listarSociosComDividas(FILE*);


#endif //PROJETO_MONTANHISMO_SOCIOS_H
