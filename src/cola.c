#include <stdlib.h>
#include "cola.h"

// crea una cola vacia
COLA crearCola(){
    COLA nuevaCola = malloc(sizeof(struct _COLA));

    nuevaCola->inicio = NULL;
    nuevaCola->final = NULL;

    return nuevaCola;
}

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

TRIE colaLeerInicio(COLA cola){
    if (cola == NULL || cola->inicio == NULL)
        return NULL;

    return cola->inicio->nodo;
}

void destruirCola(COLA cola){
    struct _COLANodo* aux = cola->inicio->sig;
    while (cola->inicio != NULL)
    {
        free(cola->inicio);
        cola->inicio = aux;
        aux = cola->inicio->sig;
    }
    free(cola);
}