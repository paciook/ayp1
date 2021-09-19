#include <stdio.h>
#include <math.h>

#define CX  0.5
#define CY  0.75
#define CZ  1.9
#define R   0.8

#define ANCHO 640
#define ALTO 480
#define FOV 90
#define PI 3.14159265

/*
 * El centro de la esfera esta en el punto (0.5,0.75,1.9) y tiene un radio de 
 * 0.8
 */



float producto_interno(float x1, float y1, float z1, float x2, float y2, float z2) {
    return (x1*x2) + (y1*y2) + (z1*z2);  // devolver producto interno
}

float norma(float x, float y, float z){
    return sqrt(producto_interno(x,y,z,x,y,z));
}

int intersecta_esfera(float dx, float dy, float dz){
    return (pow(producto_interno(CX,CY,CZ,dx,dy,dz),2) - producto_interno(CX,CY,CZ,CX,CY,CZ) + pow(R,2) >= 0);
}


int main(void){
    
    float z = ANCHO / (2 * (tan( ((FOV*PI) / 180) /2.0)));
    
    printf("P1\n%d %d\n", ANCHO, ALTO);

    for(int y = (ALTO/2)-1; y >= -ALTO/2; y--){
        for(int x = -ANCHO/2; x < ANCHO/2; x++){
            float nor = norma(x,y,z);
            float nx = x / nor;
            float ny = y / nor;
            float nz = z / nor;

            //printf("La norma es %f\n", norma(nx,ny,nz));
            printf("%d", intersecta_esfera(nx,ny,nz));
        }
    }

    return 0;
}
