#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>

// Para acceder a los hijos del TRIE usamos la funcion trieGet
struct _TRIE
{
    int esFinal;
    struct _TRIE **hijos; // no consideramos la enie
    // struct _TRIE **error; // posibles caminos si se encuentra un error
    // int profundidad; // permite determinar cuantas letras desde el final son del prefijo y cuales son errores
};

typedef struct _TRIE* TRIE;

// crea un nodo con esFinal en 0 y los punteros a hijos en nulo
TRIE crearTrie();

// funcion hash para acceder a los hijos del Trie
int hash(char letra);

// devuelve el nodo que le corresponde a la letra, o nulo si la letra no sigue al padre
TRIE trieApuntarHijo(TRIE arbol, char letra);

// todas las palabras deben estar seguidas de un \n para funcionar
// devuelve un trie armado con las palabras del archivo
TRIE trieDesdeArchivo(FILE* fp);

// genera las conecciones entre nodos que permiten identificar si se esta leyendo otro prefijo al encontrar un error
TRIE trieOptimizarDiccionario(TRIE arbol);

#endif /*  TRIE_H  */