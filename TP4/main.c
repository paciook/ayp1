#include "imagen.h"

#define MAX_ITER 80

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


int main(void) {
    color_t ambiente = {.05, .05, .05};
    vector_t origen = {0, 0, 0};

    printf("P3\n%d %d\n255\n", ANCHO, ALTO);

    float vz = ANCHO / 2 / tan(FOV/ 2 * PI / 180);

    for(int vy = ALTO / 2; vy > - ALTO / 2; vy--)
        for(int vx = - ANCHO / 2; vx < ANCHO / 2; vx++) {
            color_imprimir(computar_intensidad(ambiente, origen, vector_normalizar((vector_t){vx, vy, vz})));
            putchar('\n');
        }

    return 0;
}
