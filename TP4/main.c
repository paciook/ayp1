#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "imagen.h"
#include "vector.h"

#define MAX_ITER 80
#define FOV 90
#define PI 3.1415926535897932
#define II 255

#define BMASK 0xFF


/*
    Funciones de impresion
*/

int ftopixl(float f){
    return ((f > 1) ? 1 :f) * II;
}

void escribir_int16_little_endian(FILE *f, int16_t v){
    for(size_t c = 0; c < 16; c+=8){
        int8_t aux = (v >> c) & BMASK;
        fwrite(&aux, sizeof(int8_t), 1, f);
    }
}

void escribir_int32_little_endian(FILE *f, int32_t v){
    for(size_t c = 0; c < 32; c+=8){
        int8_t aux = (v >> c) & BMASK;
        fwrite(&aux, sizeof(int8_t), 1, f);
    }
}

void escribir_PPM(const imagen_t *imagen, FILE *f){
    size_t ancho,alto;
    imagen_dimensiones(imagen, &ancho, &alto);

    fprintf(f, "P3\n%zu %zu\n255\n", ancho, alto);

    // encabezado escrito
    
    for(size_t i = 0; i < alto; i++){
        for (size_t j = 0; j < ancho; j++){
            color_t p = imagen_get_pixel(imagen, j, i);
            fprintf(f, "%d %d %d ", ftopixl(p.r),ftopixl(p.g),ftopixl(p.b));
        }
    }
}

void escribir_BMP(imagen_t *imagen, FILE *f){
    // escribo el encabezado del archivo
    fwrite("BM", 1, 2, f);

    size_t ancho, alto, extra;
    imagen_dimensiones(imagen, &ancho, &alto);
    {
        uint32_t tamanoEncabezadoA = 14;
        uint32_t tamanoEncabezadoI = 40;
        
        size_t anchoPixel = ancho * 3;
        extra = 4 - (anchoPixel % 4);
        extra = (extra - 4) ? extra : 0;
        size_t scanline = extra ? anchoPixel + extra : anchoPixel;
        uint32_t tamanoPixeles = alto * scanline;

        uint32_t tamanoArchivo = tamanoEncabezadoA + tamanoEncabezadoI + tamanoPixeles;

        escribir_int32_little_endian(f, tamanoArchivo); //tamano del archivo
        escribir_int16_little_endian(f, 0); // reservado
        escribir_int16_little_endian(f, 0); // reservado
        escribir_int32_little_endian(f, 54); //tamano del offset
    }

    
    // escribo encabezado de la imagen
    {
        escribir_int32_little_endian(f, 40); // tamano
        escribir_int32_little_endian(f, ancho);
        escribir_int32_little_endian(f, alto);
        escribir_int16_little_endian(f, 1); // planos
        escribir_int16_little_endian(f, 24); // bits de color
        escribir_int32_little_endian(f,0); // compresion
        escribir_int32_little_endian(f,0); // tamano de imagen
        escribir_int32_little_endian(f,0); // res x
        escribir_int32_little_endian(f,0); // res y
        escribir_int32_little_endian(f,0); // tablas de color
        escribir_int32_little_endian(f,0); // colores importantes
    }


    // escribo los pixeles
    int8_t cero = 0;
    for(size_t y = alto -1; y > 0; y--){
        for(size_t x = 0; x < ancho; x++){
            color_t c = imagen_get_pixel(imagen, x, y);
            uint8_t p[] = {ftopixl(c.b), ftopixl(c.g), ftopixl(c.r)};
            fwrite(p, sizeof(int8_t), 3, f);
        }
        for(size_t i = 0; i < extra; i++)
            fwrite(&cero, sizeof(int8_t), 1, f);
    }
}


bool imagen_imprimir(char n[], bool isBin, imagen_t *img){
    FILE *f;
    typedef void (*impresion_t)(imagen_t*, FILE*);
    impresion_t imprimir;

    if(isBin){
        f = fopen(n, "wb");
        if(f == NULL) return false;

        imprimir = (impresion_t)escribir_BMP;
    } else {
        f = fopen(n, "w");
        if(f == NULL) return false;

        imprimir = (impresion_t)escribir_PPM;
    }

    imprimir(img, f);

    fclose(f);

    return true;
}

/*
    Funciones de generacion de imagen
*/

color_t computar_intensidad(color_t ambiente, vector_t o, vector_t d) {
    vector_t c = (vector_t){(d.x / d.z) * 1.5 - .5, (d.y / d.z) * 1.5 + .1, 0};
    vector_t z = {0, 0, 0};
    int n = 0;
    while(vector_norma(z) <= 2 && n < MAX_ITER) {
        z = (vector_t){z.x * z.x - z.y * z.y, z.x * z.y + z.x * z.y, 0};
        z = vector_interpolar_recta(z, c, 1);
        n++;
    }

    if(n == MAX_ITER)
        return ambiente;

    float h = (n + 1 - log(log2(vector_norma(z)))) / MAX_ITER * 360;
    float x = (1 - fabs(fmodf(h / 60.0, 2) - 1));
    float r = 0, g = 0, b = 0;

    if(h < 60)
        r = 1, g = x;
    else if(h < 120)
        g = 1, r = x;
    else if(h < 180)
        g = 1, b = x;
    else if(h < 240)
        b = 1, g = x;
    else if(h < 300)
        b = 1, r = x;
    else
        r = 1, b = x;

    return (color_t){r, g, b};
}

void print_uso(char *argv[]){
    printf("Uso: %s ANCHO ALTO <nombre de archivo BMP o PPM>", argv[0]);
}

int main(int argc, char *argv[]) {
    
    if(argc != 4){
        print_uso(argv);
        return 1;
    }

    int ancho, alto;
    if(!( (ancho = atoi(argv[1])) > 0 && (alto = atoi(argv[2])) > 0 )){
        print_uso(argv);
        return 2;
    }

    char *n_archivo = argv[3];
    bool isBinary; // True si bmp, false si ppm, error si ninguno.
    {
        size_t i = 0;
        for(; n_archivo[i]; i++);
        char *ext = &n_archivo[i-strlen(".ppm")];
        
        if(strcmp(ext,".bmp") == 0){
            isBinary = true;
        } else if(strcmp(ext,".ppm") == 0){
            isBinary = false;
        } else {
            fprintf(stderr, "Extension incorrecta\n");
            return 4;
        }
    }

    imagen_t *imagen = imagen_crear(ancho, alto);
    if(imagen == NULL){
        fprintf(stderr, "Error de memoria\n");
        return 5;
    }
    // Todas las validaciones realizadas

    color_t ambiente = {.05, .05, .05};
    vector_t origen = {0, 0, 0};

    float vz = ancho / 2 / tan(FOV/ 2 * PI / 180);
    int x,y;
    x = y = 0;
    for(int vy = alto / 2; vy > - alto / 2; vy--){
        for(int vx = - ancho / 2; vx < ancho / 2; vx++){
            color_t p = computar_intensidad(ambiente, origen, vector_normalizar((vector_t){vx, vy, vz}));
            imagen_set_pixel(imagen, x, y, p);
            x++;
        }
        x = 0;
        y++;
    }

    // Imagen generada
    
    if(!imagen_imprimir(n_archivo, isBinary, imagen)){
        fprintf(stderr, "No se pudo imprimir\n");
        return 6;
    }

    // Imagen escrita

    imagen_destruir(imagen);

    // Imagen liberada
    fprintf(stdout, "Imagen generada bajo el nombre %s\n", n_archivo);

    return 0;
}
