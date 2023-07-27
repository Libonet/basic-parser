#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "trie.h"

// devuelve un trie armado con las palabras del archivo
TRIE trieDesdeArchivo(FILE* fp);

/*utilizando el diccionario, parsea las frases de la
 entrada y las guarda bien separadas en la salida*/
void parsear(FILE* fp1, FILE* fp2, TRIE diccionario);

#endif /*  MAIN_H  */