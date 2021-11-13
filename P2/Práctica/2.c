#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct{
    float *v;
    size_t n;
} vector_t;

vector_t *vector_crear(const float vals[], size_t n){
    vector_t *aux = malloc(sizeof(vector_t));
    if(aux == NULL){
        return NULL;
    }
    aux->n = n;
    aux->v = malloc(sizeof(float)*n);
    
    if(aux->v == NULL){
        free(aux);
        return NULL;
    }

    strcpy(aux->v, vals);

    return aux;
}

void vector_destruir(vector_t *v){
    free(v->v);
    free(v);
}