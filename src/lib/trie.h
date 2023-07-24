#ifndef TRIE_H
#define TRIE_H

#include <stdlib.h>
#include <ctype.h>

// Para acceder a los hijos del TRIE usamos la funcion trieGet
struct _TRIE
{
    int esFinal;
    struct _TRIE **hijos; // no consideramos la enie
};

typedef struct _TRIE* TRIE;

TRIE crearTrie();

int hash(char letra);

TRIE trieGet(TRIE diccionario, char letra);

#endif /*  TRIE_H  */