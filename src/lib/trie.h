#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>

// Para acceder a los hijos del TRIE usamos la funcion trieApuntarHijo
struct _TRIENodo
{
    int esFinal; // determina si el nodo es un estado de aceptacion
    struct _TRIENodo **hijos;
    struct _TRIENodo *padre; // referencia al padre necesaria para la optimizacion
    struct _TRIENodo *sufijo; //apunta a un nodo del arbol que represente el camino del sufijo mas grande
    int profundidad; // permite determinar cuantas letras desde el final son del prefijo y cuales son errores
};

typedef struct _TRIENodo* TRIE;

// crea un nodo con esFinal en 0 y los punteros a hijos en nulo
TRIE crearTrie();

void destruirTrie(TRIE arbol);

// funcion hash para acceder a los hijos del Trie
int trieHash(char letra);

// devuelve el nodo que le corresponde a la letra, o nulo si la letra no sigue al padre
TRIE trieApuntarHijo(TRIE arbol, char letra);

/* devuelve un trie armado con las palabras del archivo
 (todas las palabras deben estar seguidas de un \n)*/
TRIE trieDesdeArchivo(FILE* fp);

// inserta palabras terminadas con un \n
void trieInsertarPalabra(TRIE arbol, char* palabra);

// genera las conecciones entre nodos que permiten identificar si se esta leyendo otro prefijo al encontrar un error
void trieOptimizarDiccionario(TRIE arbol);

#endif /*  TRIE_H  */