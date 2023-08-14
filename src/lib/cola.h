#ifndef COLA_H
#define COLA_H

#include "trie.h"

struct _COLANodo
{
    TRIE nodo;
    struct _COLANodo *sig;
};

typedef struct _COLA
{
    struct _COLANodo *inicio;
    struct _COLANodo *final;
} *COLA;

// crea una cola con los campos en null
COLA crearCola();

// inserta al final de la cola un nodo (tiene menor prioridad)
COLA colaAgregarAlFinal(COLA cola, TRIE nodo);

// elimina el elemento al inicio de la cola (mayor prioridad)
void colaEliminarInicio(COLA cola);

// devuelve el nodo al inicio de la cola
TRIE colaLeerInicio(COLA cola);

// libera la cola sin liberar los nodos del TRIE
void destruirCola(COLA cola);

#endif /*  COLA_H  */