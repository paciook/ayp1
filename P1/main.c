#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 3
#define M 2

#define MAX_STRING 256


/*
Ejercicio 1
-----------
dada una cadena que representa el nombre de un archivo y
otra cadena que representa su extension devuelva si el
archivo tiene esa extension o no.
*/ 
bool tiene_extension(const char archivo[], const char extension[]){
    size_t a,e;
    for(a = 0; archivo[a]; a++);
    for(e = 0; extension[e]; e++);
    printf("%c %c\n",archivo[a], extension[e]);

    if(e > a){
        return false;
    }
    
    for(size_t aux = 0; aux < e; aux++){
        if(archivo[a-aux] != extension[e - aux]){
            return false;
        }
    }

    return true;
}

/*
Ejercicio 2
-----------
Una matriz es no-negativa si todos sus elementos son mayores o
iguales a cero.
a. Escribir una función es_nonegativa que reciba una matriz de
flotantes de tamaño N×M y que retorne si es no-negativa o no.
b. Teniendo #define N 3 y #define M 2 escribir un pequeño ejemplo
de invocación que declare y defina una matriz, llame a la función
del ítem anterior e imprima un mensaje para el usuario según el
resultado.
*/
bool es_nonegativa(size_t n, size_t m, float matriz[n][m]){
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < m; j++){
            if(matriz[i][j] < 0) return false;
        }
    }
    return true;
}

/*
Ejercicio 3
-----------
Escribir un programa que le pida al usuario un grado sexagesimal
expresado como sus grados, sus minutos y sus segundos (pueden
pedirse por separado) y que imprima su representación decimal.
*/

// Main
int main(void){
    // Llamada a ejercicio 1
    
    printf("%d\n", tiene_extension("listado.txt", ".txt"));
    printf("%d\n", tiene_extension("listado.txt", ".zip"));
    

    // Llamada a ejercicio 2
    float matriz1[N][M] = {  {1,2},
                            {3,4},
                            {5,6}};
    printf("La matriz es no-negativa?: %s.\n", es_nonegativa(N,M,matriz1) ? "Si": "No");
    
    float matriz2[N][M] = {  {1,2},
                            {-3,4},
                            {5,6}};
    printf("La matriz es no-negativa?: %s.\n", es_nonegativa(N,M,matriz2) ? "Si": "No");


    /*
    Ejercicio 3
    -----------
    Escribir un programa que le pida al usuario un grado sexagesimal
    expresado como sus grados, sus minutos y sus segundos (pueden
    pedirse por separado) y que imprima su representación decimal.
    */

    float rsp = 0;

    char buff[MAX_STRING];
    printf("Ingrese la cantidad de grados\n");
    fgets(buff, MAX_STRING, stdin);
    rsp += atof(buff);

    printf("Ingrese la cantidad de minutos\n");
    fgets(buff, MAX_STRING, stdin);
    rsp += atof(buff) / 60;

    printf("Ingrese la cantidad de segundos\n");
    fgets(buff, MAX_STRING, stdin);
    rsp += atof(buff)/3600;

    printf("Representación decimal = %.4f\n", rsp);

    return 0;
}
