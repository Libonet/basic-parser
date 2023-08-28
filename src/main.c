#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "main.h"

#define FRASE_INICIAL 256
#define ERROR_INICIAL 8

SALIDA crearSalida(int capacidadFrase, int capacidadErrores){
    SALIDA salida = malloc(sizeof(struct _SALIDA));
    salida->frase = NULL;
    salida->capacidadFrase = 0;
    salida->errores = malloc(sizeof(char)*capacidadErrores);
    salida->indices = malloc(sizeof(int)*capacidadErrores);
    salida->capacidadErrores = capacidadErrores;

    return salida;
}

void destruirSalida(SALIDA salida){
    free(salida->frase);
    free(salida->errores);
    free(salida->indices);
    free(salida);
}

void fprintSalida(SALIDA salidaParseada, FILE* fpSalida){
    char letra;
    char* frase = salidaParseada->frase;
    
    fprintf(fpSalida, "%s ", frase);
    for (int i=0; (letra=salidaParseada->errores[i])!='\0'; ++i)
    {
        fprintf(fpSalida, "%c(%d) ", letra, salidaParseada->indices[i]);
    }
    fprintf(fpSalida, "\n");
}

void anotarPalabra(SALIDA salida, int* largoFrase, int* inicioPalabra, int finPalabra){

}

void anotarError(SALIDA salida, int* cantErrores, char* frase, int* inicioPalabra){
    *cantErrores++;
    if (*cantErrores > salida->capacidadErrores){
        salida->capacidadErrores *= 2;
        salida->errores = realloc(salida->errores, salida->capacidadErrores);
        salida->indices = realloc(salida->indices, salida->capacidadErrores);
    }

    salida->errores[(*cantErrores)-1] = frase[*inicioPalabra];
    salida->indices[(*cantErrores)-1] = *inicioPalabra;
    *inicioPalabra++;
}

void parsearFrase(char* frase, ssize_t nleido, SALIDA salida, TRIE diccionario, FILE* fpSalida){
    int largoFrase = 0, cantErrores = 0, profundidad=0;
    int inicioPalabra, finPalabra;
    char letra;

    TRIE estadoActual=diccionario, estadoSiguiente;

    if (nleido*2 > salida->capacidadFrase){
        salida->capacidadFrase *= 2;
        salida->frase = realloc(salida->frase, salida->capacidadFrase);
    }

    inicioPalabra = 0;
    finPalabra = -1; // se actualiza al encontrar un estado de aceptacion
    int indice = 0;

    letra = frase[indice];
    while (indice < nleido-1){
        estadoSiguiente = trieApuntarHijo(estadoActual, letra);
        if (estadoSiguiente==NULL){ // se alcanza un estado invalido
            if (finPalabra != -1){ // se encontro una palabra en el camino
                anotarPalabra(salida, &largoFrase,  &inicioPalabra, finPalabra);
                finPalabra = -1;
            }
            profundidad = estadoActual->profundidad;
            estadoActual = estadoActual->sufijo; // nos movemos al sufijo mas grande

            // la diferencia en profundidad nos da la cantidad de letras de error
            for (int i = 0; i < (profundidad-estadoActual->profundidad); i++){
                anotarError(salida, &cantErrores, frase, &inicioPalabra);
            }
        }
        else{
            indice++;
            letra = frase[indice];
        }
    }
    
    fprintSalida(salida, fpSalida);
}

void parsearArchivo(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario){
    char *linea = NULL;
    size_t largo = 0;
    ssize_t nleido;

    SALIDA salida = crearSalida(FRASE_INICIAL, ERROR_INICIAL);

    while(!feof(fpEntrada)){
        if((nleido = getline(&linea, &largo, fpEntrada)) != -1) {
            printf("Retrieved line of length %zd:\n", nleido);

            if (nleido > 1)
                parsearFrase(linea, nleido, salida, diccionario, fpSalida);
        }
    }

    destruirSalida(salida);
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
    
    parsearArchivo(fpEntrada, fpSalida, diccionario);

    fclose(fpEntrada);
    fclose(fpSalida);

    destruirTrie(diccionario);

    return 0;
}