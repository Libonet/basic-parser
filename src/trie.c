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

        arbol = trieInsertarPalabra(arbol, linea);
    }

    free(linea);

    return arbol;
}

// inserta palabras terminadas con un \n
TRIE trieInsertarPalabra(TRIE arbol, char* palabra){
    int i, profundidad=0;
    char letra;
    TRIE auxArbol = arbol, hijo;

    if (arbol == NULL){
        arbol = crearTrie();
        auxArbol = arbol;
    }

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

    return arbol;
}

void trieAhoCorasick(TRIE arbol){
    COLA cola = crearCola();
    TRIE hijo, raiz=arbol, padre, sufijo;

    arbol->sufijo=arbol;

    // el arbol nunca va a ser null pero puede no tener hijos
    if (arbol->hijos == NULL)
        return;

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

        for (int letra=0; letra<26; ++letra){ // letra = "letra"-'a' (0=a-a, 1=b-a, 2=c-a, etc)
            hijo = arbol->hijos[letra];
            if (hijo != NULL){
                colaAgregarAlFinal(cola, hijo);

                if (hijo->esFinal==0){ // si no es terminal buscamos su sufijo
                    sufijo = NULL;
                    padre = arbol; // no queremos perder la referencia al arbol
                    if (padre->sufijo->hijos != NULL)
                        sufijo = padre->sufijo->hijos[letra];
                        
                    // mientras que no encontremos el sufijo, hasta llegar a la raiz (y no encontrarlo)
                    while (sufijo==NULL && padre->sufijo!=padre){ 
                        padre = padre->sufijo;
                        if (padre->sufijo->hijos != NULL)
                            sufijo = padre->sufijo->hijos[letra];
                    }

                    if (sufijo==NULL) // si llegamos a la raiz y no lo encontramos
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