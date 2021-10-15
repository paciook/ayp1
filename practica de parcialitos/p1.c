// Parcialito 1

#include <stdio.h>
#include <stddef.h>


/*
Escribir una función que reciba una cadena de origen y extraiga de ella los elementos entre inicio (inclusivo) y fin (no
inclusivo) y los guarde como una cadena en destino. En el caso de que alguno de los índices se escape
del rango de origen se truncará al tamaño de la longitud. Siempre debe realizarse la operación, en el
peor de los casos, el resultado será una cadena vacía.
*/

void subcadena(char destino[], const char origen[], size_t inicio, size_t fin){
    size_t i;

    if(inicio >= fin)
        return;

    for(i = inicio; i < fin && (destino[i - inicio] = origen[i]); i++);


    destino[fin - inicio] = '\0';
    return;
}

/*
Escribir una función combinar_filas que reciba una matriz de flotantes de tamaño N×M, dos índices y
un multiplicador flotante. La función debe sumarle a la fila indicada por el primer índice los elementos
de la fila indicada por el segundo índice multiplicados por el multiplicador.
*/

void combinar_filas(size_t n, size_t m, float matriz[n][m], int indice_a, int indice_b, float mult){
    if(indice_a > n || indice_b > n){
        printf("ERROR, Fila out of range")
    }
    
    for(size_t p = 0; f < m; f++){
        matriz[indice_a] += mult * matriz[indice_b];
    }
    return;
}


int main(void){
    char d[20];

    subcadena(d, "Franc", 2, 6);

    printf("%s", d);

    return 0;
}
