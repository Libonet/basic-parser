#include <stdio.h>
#include "lib/trie.h"
#include "lib/main.h"

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

// Se ingresa el path a los archivos en orden: "entrada", "salida, diccionario"
int main(int argc, char** argv){
    if (argc != 4){
        printf("Cantidad de argumentos invalida\n");
        return -1;
    }

    FILE *fp = fopen(argv[3], "r"); // leemos el diccionario
    TRIE diccionario = trieDesdeArchivo(fp);
    fclose(fp);

    fp = fopen(argv[1], "r"); // leemos la entrada

    fclose(fp);

    fp = fopen(argv[2], "w"); // guardamos en la salida

    fclose(fp);

    return 0;
}