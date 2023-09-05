#include <stdlib.h>
#include "lib/cola.h"

// crea una cola vacia
COLA crearCola(){
    COLA nuevaCola = malloc(sizeof(struct _COLA));

    nuevaCola->inicio = NULL;
    nuevaCola->final = NULL;

    return nuevaCola;
}

// agrega un nodo al final de la cola
COLA colaAgregarAlFinal(COLA cola, TRIE nodo){
    if (cola == NULL)
        cola = crearCola();
    
    struct _COLANodo *nuevoNodo = malloc(sizeof(struct _COLANodo));
    nuevoNodo->nodo = nodo;
    nuevoNodo->sig = NULL;

    if (cola->final != NULL){
        cola->final->sig = nuevoNodo;

        cola->final = cola->final->sig;
        cola->final->nodo = nodo;
        cola->final->sig = NULL;
    }
    else{
        cola->final = nuevoNodo;
        cola->inicio = nuevoNodo;
    }

    return cola;
}

// elimina el primer nodo de la cola (de haberlo)
void colaEliminarInicio(COLA cola){
    if (cola == NULL || cola->inicio == NULL)
        return;

    struct _COLANodo* aux = cola->inicio->sig;

    free(cola->inicio);
    cola->inicio = aux;
    if (cola->inicio == NULL){
        cola->final = NULL;
    }
}

// devuelve la direccion del nodo (del trie) al inicio de la cola
TRIE colaLeerInicio(COLA cola){
    if (cola == NULL || cola->inicio == NULL)
        return NULL;

    return cola->inicio->nodo;
}

// libera todos los nodos de la cola (pero no del TRIE)
void destruirCola(COLA cola){
    if (cola==NULL)
        return;

    struct _COLANodo* aux;
    while (cola->inicio != NULL)
    {
        aux = cola->inicio->sig;
        free(cola->inicio);
        cola->inicio = aux;
    }
    free(cola);
}