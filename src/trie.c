#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "trie.h"
#include "cola.h"


TRIE crearTrie(){
    TRIE nuevoTrie = malloc(sizeof(struct _TRIENodo));

    nuevoTrie->esFinal = 0;
    nuevoTrie->hijos = NULL;
    nuevoTrie->padre = NULL;
    nuevoTrie->sufijo = NULL;
    nuevoTrie->profundidad = 0;

    return nuevoTrie;
}

void destruirTrie(TRIE arbol){
    if (arbol == NULL)
        return;

    if (arbol->hijos != NULL){
        for (int i=0; i<26; ++i){
            destruirTrie(arbol->hijos[i]);
        }
    }
    free(arbol);
}

// devuelve la posicion del diccionario correspondiente a la letra leida
// (asumimos que se ingresa una letra entre 'a' y 'z')
int trieHash(char letra){
    letra = tolower(letra);

    return letra-'a';
}

TRIE trieApuntarHijo(TRIE arbol, char letra){
    int pos = trieHash(letra);

    return arbol->hijos[pos];
}

TRIE trieDesdeArchivo(FILE* fpEntrada){
    int profundidad = 0;
    TRIE arbol = crearTrie();
    char *linea = NULL;
    size_t largo = 0;
    ssize_t nleido;

    while(!feof(fpEntrada)){
        if((nleido = getline(&linea, &largo, fpEntrada)) != -1){
            printf("Retrieved line of length %zd:\n", nleido);

            trieInsertarPalabra(arbol, linea);
        }
    }

    free(linea);

    return arbol;
}

// inserta palabras terminadas con un \n
void trieInsertarPalabra(TRIE arbol, char* palabra){
    int i, profundidad=0;
    char letra;
    TRIE auxArbol = arbol, hijo;

    if (arbol == NULL)
        return;

    for (i=0; (letra = palabra[i])!='\n' && letra!='\0'; ++i){
        if (letra == '\r')
            continue;

        if (auxArbol->hijos == NULL){
            auxArbol->hijos = malloc(sizeof(TRIE)*26);
            for (int i=0; i<26; i++){
                auxArbol->hijos[i] = NULL;
            }
        }

        hijo = trieApuntarHijo(auxArbol, letra);
        profundidad++;
        if (hijo == NULL)
            hijo = crearTrie();
            hijo->padre = auxArbol;
            hijo->profundidad = profundidad;

        auxArbol = hijo;
    }
}

TRIE trieOptimizarDiccionario(TRIE arbol){
    COLA cola = crearCola();
    TRIE hijo, raiz=arbol, padre, sufijo;

    if (arbol->hijos == NULL)
        return arbol;

    for (int i=0; i<26; ++i){
        hijo = arbol->hijos[i];
        if (hijo != NULL){
            colaAgregarAlFinal(cola, hijo);
            hijo->sufijo = raiz;
        }
    }

    // recorremos "breadth first" usando la cola
    while(cola->inicio!=NULL){
        arbol = colaLeerInicio(cola);
        colaEliminarInicio(cola);
        for (int i=0; i<26; ++i){ // i=letra-'a' (0=a, 1=b, etc)
            hijo = arbol->hijos[i];
            if (hijo != NULL){
                colaAgregarAlFinal(cola, hijo);

                if (hijo->esFinal==0){ // si no es terminal buscamos su sufijo
                    sufijo = padre->hijos[i];
                    while (sufijo==NULL && padre->sufijo!=NULL){
                        sufijo = padre->hijos[i];
                    }
                    hijo->sufijo = sufijo;
                } else{ // si es terminal, queremos que vuelva al estado inicial
                    hijo->sufijo = raiz;
                }
            }
        }
    }

    destruirCola(cola);
}