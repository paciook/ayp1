#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool leer_dato(uint8_t *registro, uint8_t *dato){
    if(!(*registro & 0x80)){
        *dato = (*registro >> 2) & 0x0f;
        *registro = *registro | 0x80;
        return true;
    }
    return false;
}

typedef struct {
    char *s;
    size_t len;
} cadena_t;

cadena_t *crear_cadena(const char *s){
    size_t i = 0;
    for(; s[i]; i++);

    cadena_t *cadena = malloc(sizeof(cadena_t));
    if(cadena == NULL)
        return NULL;

    cadena->s = malloc(sizeof(char)*i);

    if(cadena->s == NULL){
        free(cadena);
        return NULL;
    }
    cadena->len = i;

    cadena->s = strcpy(cadena->s, s);

    return cadena;
}

void imprimir_cadena(cadena_t c){
    for(size_t i = 0; i < c.len; i++)
        putchar(c.s[i]);
}

char *leer_digitos(){
    
}

int main(void){
    uint8_t r = 0x0f;
    uint8_t d = 0x0;
    printf("%d: %d\n", leer_dato(&r,&d), d);
    printf("%d: %d\n", leer_dato(&r,&d), d);

    r = 0xf0;
    d = 0x0;
    printf("%d: %d\n", leer_dato(&r,&d), d);
    printf("%d: %d\n", leer_dato(&r,&d), d);

    cadena_t *c = crear_cadena("Hola \ngente de youtube\n");
    imprimir_cadena(*c);

    return 0;
}
