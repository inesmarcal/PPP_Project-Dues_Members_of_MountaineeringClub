#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socios.h"

#define MAXCHAR_NOMEFICHEIRO 100

//Programa autónomo que permite visualizar o conteúdo do ficheiro binário.
//Nota: Para poder ser utlizado retirar o "espaço _" da palavra _main, ficando apenas main, utilizando o terminal.
int _main(int argc, char* argv[]) {

    FILE *fin;
    char buffer[BUFFER_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Erro: Utilização inválida (%s <nome_ficheiro>)\n", argv[0]);
        return 1;
    }

    if ((fin = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro config.txt\n");
        exit(1);
    }

    while(!feof(fin)) {

        buffer[0] = '\0';
        fread(buffer, sizeof(char), BUFFER_SIZE, fin);
        printf("%s", buffer);

    }

    fclose(fin);
    return 0;
}