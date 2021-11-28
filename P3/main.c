#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXSTRING 250

// Ejercicio 1 ##############################################################

typedef struct {
    char *nombre;
    int votos;
} candidato_t;

typedef struct {
    size_t n; // Cantidad de candidatos UNICOS
    size_t w; // Indice de un ganador
    candidato_t *can; // Si n = 0 -> can = NULL

    // Invariante de representacion:
    // w <= n
    // w >= 0
    // n >= 0
    // can tiene n elementos
} votacion_t;

bool votacion_votar(votacion_t *v, const char *nombre){
    // En caso de ya haber sido votado, hallo el indice del candidato
    size_t i;
    for(i = 0; i < v->n; i++){
        if(strcmp(v->can[i].nombre, nombre)){
            break;
        }
    }

    if(i == v->n){
        // Reservo memoria para el candidato
        {
            candidato_t *aux = realloc(v->can, sizeof(candidato_t) * (v->n + 1));
            if(aux == NULL){
                return false;
            }
            v->can = aux;
            (v->can)[i] = (candidato_t){NULL,0};
        }

        // Reservo memoria para su nombre
        {
            char *aux = malloc(sizeof(char) * strlen(nombre));
            if(aux == NULL){
                candidato_t *aux_c = realloc(v->can, sizeof(candidato_t) * v->n);
                if(aux_c != NULL){
                    v->can = aux_c;
                }
                return false;
            }
            strcpy(aux, nombre);
            v->can[i].nombre = aux;
        }
        v->n++;
    }

    // Comparo votos con el ganador hasta el momento
    if(++(v->can[i].votos) > v->can[v->w].votos){
        v->w = i;
    }

    return true;

}

char *votacion_ganador(const votacion_t *v){
    size_t w = v->w;
    candidato_t ganador = (v->can)[w];
    return ganador.nombre;
}

// ##########################################################################

// Ejercicio 2 ##############################################################

char *leer_cadena(FILE *f){
    int n;
    char str[MAXSTRING];
    str[0] = '\0';
    
    fread(&n, sizeof(int), 1, f);
    if(n != EOF){
        for(size_t i = 0; i < n; i++){
            char l;
            fread(&l, sizeof(char), 1, f);
            if(l == EOF){
                return NULL;
            }

            char caux[] = {l, '\0'};
            strcat(str, caux);
        }
    }
    
    char *r = malloc(sizeof(char) * strlen(str));
    if(r != NULL)
        strcpy(r, str);

    return r;
}

// ##########################################################################

// Ejercicio 3 ##############################################################

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Error en la cantidad de argumentos\n");
        return 1;
    }
    char *nombre = argv[1];
    int n;
    
    if((n = atoi(argv[2])) == 0){
        fprintf(stderr, "Error en la cantidad de lineas a leer\n");
        return 4;
    }

    FILE *f = fopen(nombre, "rb");
    if(f == NULL){
        fprintf(stderr, "Error al abrir el archivo\n");
        return 2;
    }

    for(int i = 0; i < n; i++){
        char *str = leer_cadena(f);
        if(str == NULL){
            fprintf(stderr, "Error en la lectura del archivo\n");
            return 3;
        }
        fprintf(stdout, "%s\n", str);
    }

    fclose(f);

    return 0;
}

// ##########################################################################
