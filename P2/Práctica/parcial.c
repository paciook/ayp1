#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

bool girar_derecha(uint8_t *r){
    uint8_t giro = 0;
    if(*r & 0x80){
        giro = ((*r >> 3) & 0x7) + 1;
    } else {
        giro = ((*r >> 3) & 0x7) - 1;
    }
    
    if(giro > 7)
        return false;
    
    *r = (*r & 0xC7) | (giro << 3);
    

    return true;
}

void imprimir_estado(uint8_t r){
    printf("D:%d , G:%d\n", (r&0x80), ((r >> 3) & 0x7));
}

char *join(const char **ss, char sep){
    
    size_t i, j, k = 0;
    for(i = 0; ss[i] != NULL; i++){
        for (j = 0; ss[i][j]; j++){
            k++;
        }
        k++;
    }
    
    char *joined = malloc(sizeof(char)*k);
    if(joined == NULL){
        return NULL;
    }

    strcpy(joined,ss[0]);
    char *aux = {sep, '\0'};
    for(size_t n = 1; n < i; n++){
        strcat(joined, aux);
        strcat(ss[n]);
    }


    return joined;
    
}

int *generar_ruido(int maximo, size_t *n){
    int num = 0;
    time_t t;
    
    srand(time(&t));
    int *r = NULL;
    
    size_t j;
    for(j = 0; (num = rand()) < maximo; j++){
        int *aux = realloc(r, sizeof(int)*(j+1));
        if(aux == NULL){
            free(r);
            return NULL;
        }
        printf("%d\n", num);
        r = aux;
        r[j] = num;
    }

    *n = j;
    return r;
}
void imprimir_lista(size_t n, int v[n]){
    for(size_t j = 0; j < n; j++){
        printf("%d\n", v[j]);
    }
}

int main(void){

    char *a = "Hola";
    char *b = "que";
    char *c = "tal";
    char *ss[] = {a,b,c, NULL};
    printf("%s", join(ss, '|'));

/*
    size_t n = 0;
    int *nums = generar_ruido(10000000, &n);
    imprimir_lista(n, nums);
    printf("%d\n", n);


    uint8_t r = 0xf0;
    imprimir_estado(r);
    if(girar_derecha(&r)){
        imprimir_estado(r);
    }
    else {
        printf("No\n");
    }

    r = 0xff;
    imprimir_estado(r);
    if(girar_derecha(&r)){
        imprimir_estado(r);
    }
    else {
        printf("No\n");
    }
*/
    return 0;
}
