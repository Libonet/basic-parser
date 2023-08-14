#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "main.h"

void fprintSalida(SALIDA salidaParseada, ssize_t largo, FILE* fpSalida){
    char letra;
    char* frase = salidaParseada->frase;
    if (salidaParseada->cantErrores==0){
        fwrite(salidaParseada->frase, sizeof(char), largo, fpSalida);
        
    }
}

SALIDA parsearPalabra(char* linea, ssize_t* largo, TRIE diccionario){
    SALIDA salidaParseada = malloc(sizeof(struct _SALIDA));
    salidaParseada->errores = NULL;
    salidaParseada->cantErrores = 0;



    return salidaParseada;
}

void parsear(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario){
    char *linea;
    size_t largo = 0;
    ssize_t nleido;
    SALIDA salidaParseada;

    while(!feof(fpEntrada)){
        linea = NULL;
        if((nleido = getline(&linea, &largo, fpEntrada)) != -1) {
            printf("Retrieved line of length %zd:\n", nleido);

            salidaParseada = parsearPalabra(linea, &nleido, diccionario);

            fprintSalida(salidaParseada, nleido, fpSalida);
        }
    }

    free(linea);
}

// Se ingresa el path a los archivos en orden: diccionario", "entrada", "salida
int main(int argc, char** argv){
    if (argc != 4){
        printf("Cantidad de argumentos invalida\n");
        return -1;
    }

    FILE *fpDiccionario = fopen(argv[1], "r"); // leemos el diccionario
    if (fpDiccionario == NULL){
        printf("No se pudo leer el archivo\n");
        exit(1);
    }
    TRIE diccionario = trieDesdeArchivo(fpDiccionario);
    fclose(fpDiccionario);

    diccionario = trieOptimizarDiccionario(diccionario);

    FILE *fpEntrada = fopen(argv[2], "r"); // leemos la entrada
    if (fpEntrada == NULL){
        printf("No se pudo leer el archivo\n");
        destruirTrie(diccionario);
        exit(1);
    }
    FILE *fpSalida = fopen(argv[3], "w"); // y guardamos en la salida al mismo tiempo
    
    parsear(fpEntrada, fpSalida, diccionario);

    fclose(fpEntrada);
    fclose(fpSalida);

    destruirTrie(diccionario);

    return 0;
}