#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
    struct nodo *sig;
    int dato;
} nodo_t;

typedef struct {
    struct nodo *prim;
} lista_ordenada_t;

bool lista_ordenada_insertar(lista_ordenada_t *l, int dato){
    // Si la lista es vacia, lo meto y listo
    if(l->prim == NULL){
        l->prim = malloc(sizeof(nodo_t));
        if(l->prim == NULL) return false;
        *(l->prim) = (nodo_t){NULL,dato};
        return true;
    }

    nodo_t *mayor;
    nodo_t *ant = NULL;
    for(mayor = l->prim; mayor != NULL; mayor = mayor->sig){
        // Si el dato de mayor, es mayor al dato, me pase y tengo que agregarlo en el anterior
        if(mayor->dato >= dato) break;

        ant = mayor;
    }

    // Creo un nodo cuyo siguiente sea el mayor, y que tenga el nuevo dato
    nodo_t *aux = malloc(sizeof(nodo_t));
    if(aux == NULL) return false;
    *aux = (nodo_t){mayor,dato};

    // Apunto el siguiente del anterior al nuevo nodo. Si es el primero, apunto la lista a aux
    if(ant == NULL) {
        l->prim = aux;
    } else {
        ant->sig = aux;
    }
    
    return true;
}

int arreglo_reducir(const int arreglo[], size_t n, int (*f)(int a, int b)){
    if(n == 0) return 0;
    
    int r;

    r = arreglo[0];
    for(size_t i = 1; i < n; i++){
        r = f(r,arreglo[i]);
    }

    return r;
}

int apariciones_de(char *cadena, char car){
    // Si la cadena esta vacia entonces no hay aparicion
    if(*cadena == '\0') return 0;

    // Compruebo el primer caracter de la cadena y le sumo las apariciones a partir del siguiente caracter
    return (cadena[0] == car) + apariciones_de(++cadena, car);
}

int suma(int a, int b){
    return a+b;
}

int prod(int a, int b){
    return a*b;
}

int main(void){
    printf("Apariciones: %d\n", apariciones_de("Hola como estas aaaaa a ", 'a'));

    lista_ordenada_t l = {NULL};
    lista_ordenada_insertar(&l, 123);
    lista_ordenada_insertar(&l, 12);
    lista_ordenada_insertar(&l, 122);
    lista_ordenada_insertar(&l, 141);
    lista_ordenada_insertar(&l, 1123141);



    for(nodo_t *n = l.prim; n != NULL; n = n->sig){
        printf("%d\n", n->dato);
    }

    int v[] = {6,123,1,12};
    size_t c = sizeof(v)/sizeof(int);
    printf("Suma %d\n",arreglo_reducir(v, c, &suma));
    printf("Producto %d\n",arreglo_reducir(v, c, &prod));

    return 0;
}
