#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socios.h"

#define MAXCHAR_NOMEFICHEIRO 100

//O programa apresenta dois ficheiros de entrada o "socios.txt" e o "quotas.txt".
//No ficheiro "socios.txt" defini que a notação seria: Nºde_socio#nome e no ficheiro "quotas.txt"
//que seria: Nºde_socio#quota. O ficheiro "quotas.txt" encontra-se apenas definido para os socios que já
//pagaram alguma quota.
//A árvore foi ordenada de acordo com o código "Nºde_socio", que defini nos ficheiros, tal como referi acima.
//Em primeiro lugar, decidi implementar uma árvore que permitia ordenar de acordo com o Nºde_socio os nomes
//dos mesmos. E posteriormente, com base nesta árvore, ao percorre-la iremos comparar se o codigo do socio
//corresponde ao da quota apresentada no ficheiro. Quando verificarmos que o codigo de sócio
//é o mesmo adicionamos ao socio o valor da quota.
//Tb estabeleci uma matriz "char error_message[BUFFER_SIZE]" que permite guardar o erro que ocorreu no ficheiro. E
//ainda defeni o codigo 100 para erros no ficheiro socios.txt e o codigo 200 para o ficheiro quotas.txt.
//De seguida, implementei as funções que permitiram obter os outputs pretendidos.
//Posteriormente, os dois ficheiros binários de saida "output1.bin"- corresponde à lista de sócios com as quotas em dia e
//o "output2.bin" corresponde à lista de sócios com as quotas em atraso.
//No ficheiro "ler.c" foi estabelecido um programa que permite observar estes mesmos outputs que se
//encontram nos ficheiros bináros. Para poder ser utlizado retirar o "espaço _" da palavra _main, ficando apenas main,
// utilizando de seguida o terminal.
//Decidi tb implementar, separadamente do programa, uma função que permite remover espaços em brancos
// (carateres espaço e tabulação) no início de uma string. Esta função encontra-se no ficheiro "mystring.c",
// cujo header file é "mystring.h".
//Os parametros de cada função encontram-se explicados no Header File "socios.h".

/***********************************************************************************************************************/
int main(int argc, char *argv[]) {

    FILE *fsocios, *fquotas, *fconfig, *fout1, *fout2;
    char filename_socios[MAXCHAR_NOMEFICHEIRO], filename_quotas[MAXCHAR_NOMEFICHEIRO];
    char ficheiro[MAXCHAR_NOMEFICHEIRO];

    // Tratamento dos parametros de entrada do programa e
    // abertura dos ficheiro de entrada (socios e quotas pagas).

    printf("Nome do ficheiro de dados dos nomes dos socios: ");
    scanf("%s", filename_socios);

    if ((fsocios = fopen(filename_socios, "r")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro %s\n", filename_socios);
        exit(1);
    }

    printf("Nome do ficheiro de dados das quotas dos socios: ");
    scanf("%s", filename_quotas);

    if ((fquotas = fopen(filename_quotas, "r")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro %s\n", filename_quotas);
        exit(1);
    }

    // Abrir o ficheiro de saída para guardar as listas de sócios com quotas em dia
    // e sócios com quotas em atraso.
    if ((fconfig = fopen(CONFIG_FILE, "r")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro config.txt\n");
        exit(1);
    }

    fgets(ficheiro, MAXCHAR_NOMEFICHEIRO, fconfig);

    // Remover o \n.
    if (ficheiro[strlen(ficheiro)-1]== '\n') {
        ficheiro[strlen(ficheiro)-1]= '\0';
    }

    if (strlen(ficheiro) <= 0 ) {
        fprintf(stderr, "Erro: Não existe ficheiro de saída em config.txt\n");
        exit(1);
    }

    if ((fout1 = fopen(ficheiro, "wb")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro output1.bin\n");
        exit(1);
    }

    fgets(ficheiro, MAXCHAR_NOMEFICHEIRO, fconfig);

    // Remover o \n.
    if (ficheiro[strlen(ficheiro)-1]== '\n') {
        ficheiro[strlen(ficheiro)-1]= '\0';
    }

    if (strlen(ficheiro) <= 0 ) {
        fprintf(stderr, "Erro: Não existe ficheiro de saída em config.txt\n");
        exit(1);
    }

    fclose(fconfig);

    if ((fout2 = fopen(ficheiro, "wb")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro output2.bin\n");
        exit(1);
    }

    //Inicialização da matriz.
    init();

    //Erros que poderão ocorrer ao não ser cumprida a formatação definida para o ficheiro "socios.txt".
    //Obter a informação correspondente ao Nºde socio e nome do mesmo no ficheiro.
    if (!loadSocios(fsocios)) {
        printf("Erro[%d]: %s\n", error_code, error_message);
        return 1;
    }

    //Erros que poderão ocorrer ao não ser cumprida a formatação definida para o ficheiro "quotas.txt".
    //Obter a informação correspondente ao Nºde socio e quota do mesmo no ficheiro.
    if (!loadQuotas(fquotas)) {
        printf("Erro[%d]: %s\n", error_code, error_message);
        return 1;
    }

    //Usada para testes (Permite fazer o print da árvore).
    //print();

    //Procedimento usado tb para testes.
    //listarSociosSemDividas(fout1);
    //listarSociosComDividas(fout2);

    //Função que permitirá listar os dois outputs pretendidos.
    listar(fout1, fout2);

    fclose(fout1);
    fclose(fout2);
    fclose(fsocios);
    fclose(fquotas);

    return 0;
}
