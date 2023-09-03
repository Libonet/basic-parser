#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/trie.h"
#include "lib/main.h"

#define FRASE_INICIAL 256
#define ERROR_INICIAL 32

SALIDA crearSalida(int capacidadFrase, int capacidadErrores){
    SALIDA salida = malloc(sizeof(struct _SALIDA));
    salida->frase = malloc(sizeof(char)*capacidadFrase);
    salida->capacidadFrase = capacidadFrase;
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
    
    fprintf(fpSalida, "%s", frase);
    if (salidaParseada->errores[0] != '\0'){
        fprintf(fpSalida, "| Errores(indice): ");
    }
    for (int i=0; (letra=salidaParseada->errores[i])!='\0'; ++i){
        fprintf(fpSalida, "%c(%d) ", letra, salidaParseada->indices[i]);
    }
    fprintf(fpSalida, "\n");
}

void anotarPalabra(SALIDA salida, int largoFrase, char* palabra, int largoPalabra){
    memcpy(salida->frase + largoFrase, palabra, largoPalabra);
}

void anotarError(SALIDA salida, int cantErrores, char* frase, int inicioPalabra){
    if (cantErrores+1 == salida->capacidadErrores){
        salida->capacidadErrores = salida->capacidadErrores * 2;
        salida->errores = realloc(salida->errores, salida->capacidadErrores);
        salida->indices = realloc(salida->indices, salida->capacidadErrores);
    }

    salida->errores[cantErrores] = frase[inicioPalabra];
    salida->errores[cantErrores+1] = '\0';
    salida->indices[cantErrores] = inicioPalabra;
}

void parsearFrase(char* frase, SALIDA salida, TRIE diccionario, FILE* fpSalida){
    int largoFrase = 0, largoPalabra=0, cantErrores = 0, profundidad = 0;
    int inicioPalabra, finPalabra, indice, bandera;
    char letra;

    TRIE estadoActual, estadoSiguiente;

    inicioPalabra = 0;
    finPalabra = -1; // se actualiza al encontrar un estado de aceptacion
    indice = 0;
    bandera = 0; // indica la finalizacion del ciclo luego de terminar el procesamiento

    letra = frase[indice];
    estadoActual = diccionario;
    while (bandera == 0){
        if (letra == '\n') // cuando se alcanza el \n completamos un ultimo ciclo
            bandera = 1; // utilizamos este ciclo para reconocer la ultima palabra y los errores que quedan

        // reconocemos la letra
        estadoSiguiente = trieApuntarHijo(estadoActual, letra);

        if (estadoSiguiente != NULL){ // alcanzamos un estado valido
            estadoActual = estadoSiguiente; // avanzamos al hijo (la letra leida)
            if (estadoActual->esFinal == 1) // si es un final de palabra, guardamos su indice
                finPalabra = indice;
            
            indice++;
            letra = frase[indice]; // obtenemos la siguiente letra
        }
        else{ // se alcanza un estado invalido
            largoPalabra = 0;
            if (finPalabra != -1){ // si se encontro una palabra completa en el camino, la anotamos
                largoPalabra = (finPalabra+1 - inicioPalabra);
                anotarPalabra(salida, largoFrase, frase+inicioPalabra, largoPalabra);
                largoFrase += largoPalabra + 1; // largo de la palabra + 1 espacio
                salida->frase[largoFrase-1] = ' ';

                inicioPalabra = finPalabra+1;
                finPalabra = -1;
            }

            // si nuestro estado es la raiz, entonces encontramos un error al leer la primer letra
            // por lo que necesitamos anotar el error y avanzar el indice
            if (estadoActual == diccionario && letra!='\n'){ // evitamos anotar el \n
                anotarError(salida, cantErrores, frase, inicioPalabra);
                cantErrores++;
                inicioPalabra++;

                indice++;
                letra = frase[indice]; // obtenemos la siguiente letra
                continue;
            }

            // calculamos la cantidad de errores a traves de la diferencia entre profundidad con 
            // el sufijo. Ademas, le restamos la palabra que se extrajo (si la hay)
            profundidad = estadoActual->profundidad - largoPalabra;
            estadoActual = estadoActual->sufijo; // nos movemos al sufijo mas grande
            
            // la diferencia en profundidad nos da la cantidad de letras de error
            for (int i = 0; i < (profundidad-estadoActual->profundidad); i++){
                anotarError(salida, cantErrores, frase, inicioPalabra);
                cantErrores++;
                inicioPalabra++;
            }
        }
    }
    
    salida->frase[largoFrase] = '\0';
    fprintSalida(salida, fpSalida);
}

void parsearArchivo(FILE* fpEntrada, FILE* fpSalida, TRIE diccionario){
    char *linea = NULL;
    size_t largo = 0;
    ssize_t nleido;

    SALIDA salida = crearSalida(FRASE_INICIAL, ERROR_INICIAL);

    // getline reserva la memoria necesaria para leer cada linea del archivo
    while((nleido = getline(&linea, &largo, fpEntrada)) != -1) {
        // printf("Retrieved line of length %zd:\n", nleido);

        if (nleido*2 > salida->capacidadFrase - 1){
            salida->capacidadFrase *= 2;
            salida->frase = realloc(salida->frase, salida->capacidadFrase);
        }

        if (nleido > 1){
            salida->frase[0] = '\0';
            salida->errores[0] = '\0';
            parsearFrase(linea, salida, diccionario, fpSalida);
        }
    }

    destruirSalida(salida);
    free(linea);
}

// Se ingresa el path a los archivos en orden: diccionario", "entrada", "salida"
int main(int argc, char** argv){
    if (argc != 4){
        printf("Cantidad de argumentos invalida\n");
        exit(1);
    }

    FILE *fpDiccionario = fopen(argv[1], "r"); // leemos el diccionario
    if (fpDiccionario == NULL){
        printf("No se pudo leer el archivo\n");
        exit(1);
    }

    TRIE diccionario = trieDesdeArchivo(fpDiccionario);
    fclose(fpDiccionario);

    trieOptimizarDiccionario(diccionario);

    FILE *fpEntrada = fopen(argv[2], "r"); // leemos la entrada
    if (fpEntrada == NULL){
        printf("No se pudo leer el archivo\n");
        // guardarTrie(diccionario);
        destruirTrie(diccionario);
        exit(1);
    }
    FILE *fpSalida = fopen(argv[3], "w"); // y guardamos en la salida al mismo tiempo

    parsearArchivo(fpEntrada, fpSalida, diccionario);

    fclose(fpEntrada);
    fclose(fpSalida);

    // guardarTrie(diccionario);
    destruirTrie(diccionario);

    return 0;
}