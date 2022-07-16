#include <stdio.h>

// Remove espaço em branco (carateres espaço e tabulação) no início de uma string.
//Parametro: string.
char *ltrim(char *s) {

    int i, j;

    //Encontra-se vazia.
    if (!s) return NULL;

    // Percorrer a string "s" enquanto: 1) Não terminar e 2) tiver um espaço ou tabulação.
    for (i = 0; s[i] && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'); i++) {

        if (s[i+1] == '\0') s[i] = '\0';

        for (j = i; s[j]; j++) {

            s[j] = s[j+1];
        }
    }

    return s;
}