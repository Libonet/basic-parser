#include <stdlib.h>
#include <ctype.h>
#include "lib/trie.h"

TRIE crearTrie(){
    TRIE nuevoTrie = malloc(sizeof(struct _TRIE));

    nuevoTrie->esFinal = 0;
    nuevoTrie->hijos = malloc(sizeof(TRIE)*26);
    for (int i=0; i<26; i++){
        nuevoTrie->hijos[i] = NULL;
    }

    return nuevoTrie;
}

int hash(char letra){ // asumimos que se ingresa una letra entre 'a' y 'z'
    letra = tolower(letra);

    return letra-'a';
}

TRIE trieApuntarHijo(TRIE arbol, char letra){
    int pos = hash(letra);

    return arbol->hijos[pos];
}