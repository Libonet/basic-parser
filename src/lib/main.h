#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "trie.h"

/*utilizando el diccionario, parsea las frases de la
 entrada y las guarda bien separadas en la salida*/
void parsear(FILE* fp1, FILE* fp2, TRIE diccionario);

#endif /*  MAIN_H  */