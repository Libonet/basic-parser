#include <stdio.h>
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

// todas las palabras deben estar seguidas de un \n para funcionar
TRIE trieDesdeArchivo(FILE* fp){
    char c;
    TRIE arbol = crearTrie();
    TRIE aux = arbol;
    
    while(!feof(fp)){
        c = fgetc(fp);
        if (c == '\r')
            continue;
        if (c == '\n'){
            aux->esFinal = 1;
            aux = arbol;
        }
        else{
            aux = trieApuntarHijo(aux, c);
            if (aux == NULL)
                aux = crearTrie();
        }
    }

    return arbol;
}

