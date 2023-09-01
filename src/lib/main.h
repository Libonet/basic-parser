#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "trie.h"

// Utilizamos esta estructura para almacenar temporalmente las frases
// sin necesidad de volver a reservar memoria para cada una
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

void anotarPalabra(SALIDA salida, int largoFrase, char* palabra, int largoPalabra);

void anotarError(SALIDA salida, int cantErrores, char* frase, int inicioPalabra);

void parsearFrase(char* frase, SALIDA salida, TRIE diccionario, FILE* fpSalida);

/*utilizando el diccionario, parsea las frases de la
 entrada y las guarda bien separadas en la salida*/
void parsearArchivo(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario);

#endif /*  MAIN_H  */