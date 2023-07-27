#include <stdio.h>
#include "trie.h"
#include "main.h"

void parsear(FILE* fp1, FILE* fp2, TRIE diccionario){
    
}

// Se ingresa el path a los archivos en orden: "entrada", "salida, diccionario"
int main(int argc, char** argv){
    if (argc != 4){
        printf("Cantidad de argumentos invalida\n");
        return -1;
    }

    FILE *fp1 = fopen(argv[3], "r"); // leemos el diccionario
    TRIE diccionario = trieDesdeArchivo(fp1);
    fclose(fp1);

    // diccionario = trieOptimizarDiccionario(diccionario);

    fp1 = fopen(argv[1], "r"); // leemos la entrada
    FILE *fp2 = fopen(argv[2], "w"); // y guardamos en la salida al mismo tiempo
    
    parsear(fp1, fp2, diccionario);

    fclose(fp1);
    fclose(fp2);



    return 0;
}