#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "trie.h"

typedef struct _SALIDA
{
    char* frase;
    char** errores;
    int cantErrores;
} *SALIDA;

// escribe en el archivo al que apunta "fpSalida" el contenido de "salidaParseada", formateandolo
void fprintSalida(SALIDA salidaParseada, ssize_t largo, FILE* fpSalida);

SALIDA parsearPalabra(char* linea, ssize_t* largo, TRIE diccionario);

/*utilizando el diccionario, parsea las frases de la
 entrada y las guarda bien separadas en la salida*/
void parsear(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario);

#endif /*  MAIN_H  */