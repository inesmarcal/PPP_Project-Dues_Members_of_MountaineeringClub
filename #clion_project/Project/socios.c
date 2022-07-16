#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socios.h"
#include "mystring.h"

// Nota: A árvore foi ordenada de acordo com o codigo "Nº de socio", que defini nos ficheiros.
// No ficheiro "socios.txt" defini que a notação seria: Nºde_socio#nome e no ficheiro quotas.txt
// que seria: Nºde_socio#quota.

static int raiz = -1;
//Matriz para os socios.
static Tnode matriz[MAX_SOCIOS];


//Inicialização da matriz.
void init(){
    int i;

    raiz = -1;

    for(i = 0; i < MAX_SOCIOS; i++)
        matriz[i].disponivel = 1; // 0 - Não disponível, 1 - Disponível
}

//Função que permite criar o socio a partir dos dados do ficheiro.
Socio criar_socio(int codigo, char *nome){
    Socio novo;

    novo.codigo = codigo;
    strcpy(novo.nome, nome);
    novo.quota = 0;

    return novo;
}

//Função que permite colocar o socio na matriz de acordo com a disponibilidade da mesma.
int colocar(Socio socio){
    int i;

    // Procurar primeira posição disponível na matriz.
    for (i = 0; i < MAX_SOCIOS && !matriz[i].disponivel; i++);

    if (i == MAX_SOCIOS) return 0; // Não é possível colocar (não há espaço na matriz).

    matriz[i].disponivel = 0; // Passou a ocupado.
    matriz[i].socio = socio;
    matriz[i].left = matriz[i].right = -1;

    raiz = addTree(raiz, i);

    return 1; // Colocou o sócio com sucesso.
}

//Árvore que permitirá colocar o nome dos socios que foram inseridos na matriz ordenados
//de acordo com o Nºde_socio explicado anteriormente.
int addTree(int no, int novo){

    if (no == -1) return novo;
    else {

        if (matriz[no].socio.codigo > matriz[novo].socio.codigo) {

            matriz[no].left = addTree(matriz[no].left, novo);

        } else {

            matriz[no].right = addTree(matriz[no].right, novo);
        }
    }

    return no;
}

//Permitirá colocar a quota nos respetivos socios (verifica se o codigo do socio corresponde ao codigo da quota,
// para assim a poder introduzir corretamente).
int inserir_quota_arvore(int no, int socio, double quota){

    if (no == -1) return 0;

    if (matriz[no].socio.codigo == socio) {

        matriz[no].socio.quota += quota;

        return 1;

    } else {

        if (matriz[no].socio.codigo > socio) {

            return inserir_quota_arvore(matriz[no].left, socio, quota);

        } else {

            return inserir_quota_arvore(matriz[no].right, socio, quota);
        }
    }
}

int inserir_quota(int socio, double quota){

    return inserir_quota_arvore(raiz, socio, quota);

}

//Função que nos permitirá visualizar a árvore da esquerda para a direita.
void printTree(int no){

    if (no == -1) return;

    printTree(matriz[no].left);
    printf("%d, %s, %.2lf €\n", matriz[no].socio.codigo, matriz[no].socio.nome, matriz[no].socio.quota);
    printTree(matriz[no].right);
}

// Procedimento utilizado para testes (visualização da árvore).
/*void print()
{

    if (raiz == -1) {
        printf("Não existem dados...\n");
    } else {
        printTree(raiz);
    }

}*/

//Permite obter o nome dos socios definidos no ficheiro "socios.txt".
//Caso tb insiramos os dados mal no ficheiro, ou seja não estão de acordo com a notação referida,
//esta função irá detetar o tipo de erro(Foi definido um numero para cada erro).
//Erro= 100 e posterimente este numero incrementado irá corresponder a um erro diferente no ficheiro "socios.txt".
int loadSocios(FILE *file){

    char buffer[BUFFER_SIZE], *token, nome[MAXCHAR_NOMESOCIO], *ptr, *nome_socio;
    int codigo;
    int error = 100;

    // Ler o ficheiro linha a linha.
    while (fgets(buffer, BUFFER_SIZE, file)) {

        token = strtok(buffer, "#");

        //Para entrar nesta condição a string tem de apresentar o carater que determina o final
        //da string, o '\0', que se não encontra-se vazia e o socio nao tem dados.
        //Função "char *ltrim(char*)" encontra-se no ficheiro "mystring.c" com Header File no "mystring.h"

        if (strlen(ltrim(token)) > 0) {

            codigo = (int) strtol(token, &ptr, 10);

            //Caso a conversão do Nºde_socio com a função "strtol" não corra bem, é definido como erro.
            if (codigo == 0) {
                error_code = error + 1;
                strcpy(error_message, "Não foi possível converter o codigo de sócio.");
                return 0;
            }

            token = strtok(NULL, "#");
            //Caso a seguir ao Nºde_socio não corra bem retirar o # ou o mesmo
            //não exista, é selecionado como erro,.
            if (token == NULL) {
                error_code = error + 2;
                strcpy(error_message, "Formato do ficheiro inválido (<código de sócio>#<nome de socio>).");
                return 0;
            }
            //Copiamos o nome se existir.
            nome_socio= strcpy(nome, token);
            // Remover o \n.
            nome[strlen(nome) - 1] = '\0';

            //Caso o nome não exista é detetado erro.
            if (strcasecmp(nome_socio, "") == 0 ){
                error_code = error + 3;
                strcpy(error_message, "Não existe nome.");
                return 0;
            }

            token = strtok(NULL, "#");

            //Caso ainda existe mais algum # o formato do ficheiro é invalido(não foi definido
            //de acordo com as condições referidas anteriormente).
            if (token != NULL) {
                error_code = error + 4;
                strcpy(error_message, "Formato do ficheiro inválido (<código de sócio>#<nome de socio>).");
                return 0;
            }


            // Não é possível colocar (não há espaço na matriz).
            if (!colocar(criar_socio(codigo, nome))) {
                error_code = error + 5;
                strcpy(error_message, "Não existe espaço.");
                return 0;
            }
        }
    }

    return 1; // Correu bem.
}

//Permite obter as quotas dos socios, de acordo com a estrutura definida no ficheiro "quotas.txt".
//Caso tb insiramos os dados mal no ficheiro, ou seja não estão de acordo com a notação referida,
//esta função irá detetar o tipo de erro(Foi definido um numero para cada erro).
//Erro= 200 e posterimente este numero incrementado irá corresponder a um erro diferente no ficheiro "quotas.txt".
int loadQuotas(FILE * file){

    char buffer[BUFFER_SIZE], *token, *ptr;
    int codigo;
    double quota;
    int error = 200;


    // Ler o ficheiro linha a linha.
    while (fgets(buffer, BUFFER_SIZE, file)) {

        token = strtok(buffer, "#");

        if (strlen(ltrim(token)) > 0) {


            codigo = (int) strtol(token, &ptr, 10);

            //Caso a conversão do Nºde_socio com a função "strtol" não corra bem, é definido como erro.
            if (codigo == 0) {

                error_code = error + 1;
                strcpy(error_message, "Não foi possível converter o codigo de sócio.");
                return 0;
            }

            token = strtok(NULL, "#");
            //Caso a seguir ao Nºde_socio não corra bem retirar o # ou o mesmo
            // não exista é selecionado como erro.
            if (token == NULL) {

                error_code = error + 2;
                strcpy(error_message, "Formato do ficheiro inválido (<código de sócio>#<quota paga>).");
                return 0;
            }

            //Ocorreu um erro ao converter o valor da quota.
            quota = (float) strtod(token, &ptr);

            if (quota == 0) {

                error_code = error + 3;
                strcpy(error_message, "Não foi possível converter o valor da quota.");
                return 0;
            }

            token = strtok(NULL, "#");
            //Caso ainda existe mais algum # o formato do ficheiro é invalido(não foi definido
            //de acordo com as condições referidas anteriormente).
            if (token != NULL) {
                error_code = error + 4;
                strcpy(error_message, "Formato do ficheiro inválido (<código de sócio>#<quota paga>).");
                return 0;
            }

            //Não foi possivel inserir a quota do socio.
            if (!inserir_quota(codigo, quota)) {

                error_code = error + 5;
                strcpy(error_message, "Não foi possivel inserir a quota do socio.");
                return 0;
            }
        }
    }
    return 1; //Correu bem.
}

//Função que permite selecionar os socios que tem as quotas em dia e o eventual
//valor do montante pago a mais que poderá posteriormente ser utilizado para os anos seguintes.
void printTreeSociosSemDividas(FILE *file, int no){

    char buffer[BUFFER_SIZE];

    if (no == -1) return;

    printTreeSociosSemDividas(file, matriz[no].left);

    if (matriz[no].socio.quota >= QUOTA_ANUAL) {

        sprintf(buffer, "N.º Sócio: %d, Nome: %s, Crédito: %.2lf €\n",
                matriz[no].socio.codigo,
                matriz[no].socio.nome,
                matriz[no].socio.quota - QUOTA_ANUAL);

        //Permite visualizar o output.
        //printf("%s", buffer);

        fwrite(buffer, sizeof(char), BUFFER_SIZE, file); //Escreve no ficheiro.

    }
    printTreeSociosSemDividas(file, matriz[no].right);
}

//Função que permite selecionar os socios com quotas em atraso e o correspondente
// valor da divida dos mesmos.
void printTreeSociosComDividas(FILE *file, int no){

    char buffer[BUFFER_SIZE];

    if (no == -1) return;

    printTreeSociosComDividas(file, matriz[no].left);

    if (matriz[no].socio.quota < QUOTA_ANUAL) {

        sprintf(buffer, "N.º Sócio: %d, Nome: %s, Dívida: %.2lf €\n",
                matriz[no].socio.codigo,
                matriz[no].socio.nome,
                matriz[no].socio.quota - QUOTA_ANUAL);

        //Permite visualizar o output.
        //printf("%s", buffer);

        fwrite(buffer, sizeof(char), BUFFER_SIZE, file); //Escreve no ficheiro.

    }
    printTreeSociosComDividas(file, matriz[no].right);
}

//Permite listar os outputs da função "printTreeSociosSemDividas" caso existam dados na mesma.
void listarSociosSemDividas(FILE *file){

    char buffer[BUFFER_SIZE];

    if (raiz == -1) {

        sprintf(buffer,"Não existem dados...\n");
        fwrite(buffer, sizeof(char), BUFFER_SIZE, file); //Escreve no ficheiro que não existem dados.

    } else {
        printTreeSociosSemDividas(file, raiz); //Caso contrario escreve os dados existentes.
    }

}

//Permite listar os outputs da função "printTreeSociosComDividas" caso existam dados na mesma.
void listarSociosComDividas(FILE *file){

    char buffer[BUFFER_SIZE];

    if (raiz == -1) {

        sprintf(buffer,"Não existem dados...\n");
        fwrite(buffer, sizeof(char), BUFFER_SIZE, file); //Escreve no ficheiro que não existem dados.

    } else {
        printTreeSociosComDividas(file, raiz); //Caso contrario escreve os dados existentes.
    }


}

//Listagem organizada dos dados obtidos através das funções definidas anteriormente.
//Permitirá depois escrever nos ficheiros binários.
void listar(FILE *semdivida, FILE *comdivida){

    char buffer[BUFFER_SIZE];

    sprintf(buffer, "Lista de sócios com as quotas em dia:\n\n");
    fwrite(buffer, sizeof(char), BUFFER_SIZE, semdivida);

    listarSociosSemDividas(semdivida);
    //Chama assim as funções que permitem listar os outputs correspondentes aos socios sem dividas.

    sprintf(buffer, "\n\nLista de sócios com as quotas em atraso:\n\n");
    fwrite(buffer, sizeof(char), BUFFER_SIZE, comdivida);

    listarSociosComDividas(comdivida);
    //Chama assim as funções que permitem listar os outputs correspondentes aos socios com dividas.
}
