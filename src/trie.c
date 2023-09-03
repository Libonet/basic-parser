#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/trie.h"
#include "lib/cola.h"


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
        free(arbol->hijos);
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
    if (arbol == NULL || arbol->hijos == NULL || letra=='\n')
        return NULL;

    int pos = trieHash(letra);

    return arbol->hijos[pos];
}

TRIE trieDesdeArchivo(FILE* fpEntrada){
    TRIE arbol = crearTrie();
    char *linea = NULL;
    size_t largo = 0;
    ssize_t nleido;

    while((nleido = getline(&linea, &largo, fpEntrada)) != -1){
        // printf("Retrieved line of length %zd:\n", nleido);

        trieInsertarPalabra(arbol, linea);
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
        if (hijo == NULL){
            hijo = crearTrie();
            auxArbol->hijos[trieHash(letra)] = hijo;

            hijo->padre = auxArbol;
            hijo->profundidad = profundidad;
        }

        auxArbol = hijo;
    }

    auxArbol->esFinal = 1;
}

void trieOptimizarDiccionario(TRIE arbol){
    COLA cola = crearCola();
    TRIE hijo, raiz=arbol, padre, sufijo;

    if (arbol==NULL || arbol->hijos == NULL)
        return;

    arbol->sufijo=arbol;
    for (int i=0; i<26; ++i){
        hijo = arbol->hijos[i];
        if (hijo != NULL){
            colaAgregarAlFinal(cola, hijo);
            hijo->sufijo = raiz;
        }
    }

    // recorremos "breadth first" usando la cola
    while(cola->inicio != NULL){
        arbol = colaLeerInicio(cola);
        colaEliminarInicio(cola);
        if (arbol->hijos == NULL)
            continue;

        for (int i=0; i<26; ++i){ // i=letra-'a' (0=a, 1=b, etc)
            hijo = arbol->hijos[i];
            if (hijo != NULL){
                colaAgregarAlFinal(cola, hijo);

                if (hijo->esFinal==0){ // si no es terminal buscamos su sufijo
                    sufijo = NULL;
                    padre = arbol; // no queremos perder la referencia al arbol
                    if (padre->sufijo->hijos != NULL)
                        sufijo = padre->sufijo->hijos[i];
                        
                    // mientras que no encontremos el sufijo, y hasta llegar a la raiz
                    while (sufijo==NULL && padre->sufijo!=padre){ 
                        padre = padre->sufijo;
                        if (padre->sufijo->hijos != NULL)
                            sufijo = padre->sufijo->hijos[i];
                    }

                    if (sufijo==NULL)
                        sufijo = raiz;
                    
                    hijo->sufijo = sufijo;

                } else{ // si es terminal, queremos que vuelva al estado inicial directamente
                    hijo->sufijo = raiz;
                }
            }
        }
    }

    destruirCola(cola);
}