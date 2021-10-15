// Parcialito 1

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

/*
Escribir una función
int mi_atoi(const char s[]);
que dada una cadena de caracteres s devuelva el número entero contenido en ella. No deben
realizarse validaciones.
Por ejemplo, si se llara a la función con "75029511" como s la función devolvería el
entero 75029511.
*/

int mi_atoi(const char s[]){
    int n = 0;
    size_t i = 0;
    bool es_negativo = false;
    
    if(s[0] == '-'){
        es_negativo = true;
        i++;
    }

    for(; s[i]; i++){
        n = n*10 + s[i] - '0';
    }

    return es_negativo ? -n : n;
}

/*
Una matriz tiene paridad si los elementos de las posiciones pares son pares y los de las posiciones
impares son impares, donde la posición es la suma de la fila y la columna.
*/

bool tiene_paridad(int n, int m, int matriz[n][m]){

    for(size_t x = 0; x < n; x++){
        for(size_t y = 0; y < m; y++){
            if((x+y)%2 - matriz[x][y]%2)
                return false;
        }
    }
    return true;
}

/*
El pH de una solución se calcula en función de la concentración de iones hidronio (H3O+) según
la fórmula:
pH =           1
        log( ------ )
             [H3O+]
Escribir un programa que le pida al usuario una concentración de hidronio y que imprima el pH
correspondiente a dicha concentración.
Realizar todas las validaciones pertinentes.
*/

void ph_of(float hidronio){

    if(hidronio <= 0 || hidronio){
        printf("Imposible mi rey. No sé dividir por 0 ni calcular logaritmos de números negativos");
        return;
    }

    float ph = log10(1/hidronio);

    printf("%.7f", ph);

    return;
}

int main(void){

    char i[] = "-123";

    printf("%d", mi_atoi(i));



/*
    int d[3][4] = {{4,9,6,7},
                    {5,8,1,8},
                    {0,2,3,1}};

    printf("%d", tiene_paridad(3,4,d));
*/

/*
    char s[20];
    fgets(s, 100, stdin);
    float h = atof(s);
    ph_of(h);
    return 0;
*/
}
