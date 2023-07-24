#include <stdio.h>
#include "lib/trie.h"
#include "lib/main.h"

TRIE trieDesdeArchivo(FILE archivo){

}

// Se ingresa el path a los archivos en orden: "entrada", "salida, diccionario"
int main(int argc, char** argv){
    if (argc != 4){
        printf("Cantidad de argumentos invalida\n");
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    TRIE trie = trieDesdeArchivo(fp);

    return 0;
}