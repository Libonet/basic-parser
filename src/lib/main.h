#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "trie.h"

typedef struct _SALIDA
{
    char* frase;
    int capacidadFrase;
    char* errores;
    int* indices; // lista de indices en donde se encontraron errores
    int capacidadErrores;
} *SALIDA;

SALIDA crearSalida(int capacidadFrase, int capacidadErrores);

void destruirSalida(SALIDA salida);

// escribe en el archivo al que apunta "fpSalida" el contenido de "salidaParseada", bien formateado
void fprintSalida(SALIDA salidaParseada, FILE* fpSalida);

void anotarPalabra(SALIDA salida, char* frase, int largoFrase, int inicioPalabra, int finPalabra);

void anotarError(SALIDA salida, int cantErrores, char* frase, int inicioPalabra);

void parsearFrase(char* frase, ssize_t nleido, SALIDA salida, TRIE diccionario, FILE* fpSalida);

/*utilizando el diccionario, parsea las frases de la
 entrada y las guarda bien separadas en la salida*/
void parsearArchivo(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario);

#endif /*  MAIN_H  */