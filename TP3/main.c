#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>
#include <assert.h>

#define INFINITO FLT_MAX

#define IA  5
#define II  255

#define FOV 90
#define PI 3.1415926535897932
#define ANCHO 640
#define ALTO 480

// Estructuras #######################################

typedef struct {
    float x, y, z;
} vector_t;

typedef struct {
    float r, g, b;
} color_t;

typedef struct {
    vector_t centro;
    float radio;
    color_t color;
    float ka, kd;

} esfera_t;

typedef struct {
    vector_t posicion; // Si es_puntual es una coordenada, si no una dirección
    color_t color;
    bool es_puntual;
} luz_t;

typedef struct {
    void **v;
    size_t n;
} arreglo_t;

// ###################################################

// Funciones #########################################

float vector_producto_interno(vector_t a, vector_t b){
    float prod = 0;
    prod += a.x * b.x;
    prod += a.y * b.y;
    prod += a.z * b.z;

    return prod;
}


float vector_norma(vector_t a){
    return sqrt(vector_producto_interno(a,a));
}

vector_t vector_resta(vector_t a, vector_t b){
    vector_t r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

vector_t vector_interpolar_recta(vector_t o, vector_t d, float t){
    vector_t p = {o.x + (d.x)*t, o.y + (d.y)*t, o.z + (d.z)*t};
    return p;
}

vector_t vector_normalizar(vector_t a){
    vector_t n;
    float norma = vector_norma(a);
    n.x = a.x / norma;
    n.y = a.y / norma;
    n.z = a.z / norma;

    return n;
}

color_t color_sumar(color_t c, color_t m, float p){
    color_t s;
    s.r = c.r + p*m.r;
    s.g = c.g + p*m.g;
    s.b = c.b + p*m.b;
    return s;
}

color_t color_absorber(color_t b, color_t c){
    color_t a;
    a.r = c.r * b.r;
    a.g = c.g * b.g;
    a.b = c.b * b.b;
    return a;
}

int ftoppm(float f){
    return ((f > 1) ? 1 :f) * II;
}

void color_imprimir(color_t c){
    printf("%d %d %d ", ftoppm(c.r),ftoppm(c.g),ftoppm(c.b));
}

esfera_t *esfera_crear(vector_t centro, float radio, color_t color,
                        float ka, float kd){
    esfera_t *e = malloc(sizeof(esfera_t));
    if(e == NULL)
        return NULL;

    e->centro = centro;
    e->radio = radio;
    e->color = color;
    e->ka = ka;
    e->kd = kd;

    return e;
}

void esfera_destruir(esfera_t *esfera){
    free(esfera);
}

luz_t *luz_crear(vector_t posicion, color_t color, bool es_puntual){
    luz_t *l = malloc(sizeof(luz_t));
    if(l == NULL)
        return NULL;

    l->posicion = posicion;
    l->color = color;
    l->es_puntual = es_puntual;

    return l;
}

void luz_destruir(luz_t *luz){
    free(luz);
}

float esfera_distancia(const esfera_t *esfera, vector_t o, vector_t d,
                        vector_t *punto, vector_t *normal){
    float t = INFINITO;
    
	float disc = 0;

    vector_t res = vector_resta(esfera->centro,o);
    float p_resd = vector_producto_interno(res,d);
    float rcuad = (esfera->radio)*(esfera->radio);
	disc = (p_resd*p_resd) - vector_producto_interno(res,res) + rcuad;
    
    if(disc >= 0){
        float sq = sqrt(disc);
        t = p_resd + ((p_resd <= sq) ? sq : -sq);
    }
    printf("%f\n", t);
    if(t <= 0){
        return INFINITO;
    }

    *punto = vector_interpolar_recta(o,d,t);
    *normal = vector_normalizar(vector_resta(*punto,esfera->centro));

    return t;
}

bool arreglo_agregar(arreglo_t *a, void *e){
    void *aux = realloc((a->v), ((a->n) + 1)*sizeof(void*));
    if(aux==NULL){
        return false;
    }

    a->v = aux;
    (a->v)[a->n] = e;
    a->n += 1;
    return true;
}

color_t computar_intensidad(const arreglo_t *esferas, const arreglo_t *luces,
                            color_t ambiente, vector_t o, vector_t d){
    color_t c = {0,0,0};
    color_t ia = {IA/255.0,IA/255.0,IA/255.0};
    vector_t p,n;
    float t,ant;
    size_t n_esf;
    t=ant=INFINITO;

    for(size_t i = 0; i < esferas->n; i++){
        ant = esfera_distancia((esferas->v)[i], o, d, &p, &n);
        if(ant < t){
            t = ant;
            n_esf = i;
        }
    }

    if(t==INFINITO){
        return c;
    }

    esfera_t *esfera = ((esfera_t*)((esferas->v)[n_esf]));
    for(size_t i = 0; i < luces->n; i++ ){
        luz_t *luz = ((luz_t*)((luces->v)[i]));
        
        color_t inten_i = color_absorber(luz->color,esfera->color);
        vector_t l_luz = (luz->es_puntual) ? vector_resta(luz->posicion,p) : luz->posicion;
        float prod_ln = vector_producto_interno(l_luz,n);

        color_sumar(c, inten_i, esfera->kd *  prod_ln);
    }
    color_sumar(c, ia, ((esfera_t*)((esferas->v)[n_esf]))->ka);

    return c;
}
// ###################################################

// Main ##############################################

int main(void) {
    arreglo_t luces = {NULL, 0};
    arreglo_t esferas = {NULL, 0};

    color_t ambiente = {.05, .05, .05};

    assert(arreglo_agregar(&luces, luz_crear(vector_normalizar((vector_t){0, 1, 0}), (color_t){.2, .2, .2}, false)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){-2, 10, 2.5}, (color_t){.9, 0, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){2, 10, 2.5}, (color_t){0, .9, 0}, true)));
    assert(arreglo_agregar(&luces, luz_crear((vector_t){0, 10, 5}, (color_t){0, 0, .9}, true)));

    assert(luces.n == 4);
    for(size_t i = 0; i < luces.n; i++)
        assert(luces.v[i] != NULL);

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1, 2.4}, .3, (color_t){1, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.4, 3}, 1, (color_t){1, 1, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, -.6, 3}, .3, (color_t){1, 0, 0}, 1, .8)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1.73, -.6, 2}, .3, (color_t){1, 1, 0}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, -.6, 1.26}, .3, (color_t){0, 1, 0}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.6, 1}, .3, (color_t){1, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, -.6, 1.26}, .3, (color_t){0, 1, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1.73, -.6, 2}, .3, (color_t){0, 0, 1}, 1, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, -.6, 3}, .3, (color_t){1, 0, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 0)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .16)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .5)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .66)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .83)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 1)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 1.5, 4}, .3, (color_t){1, 1, 1}, 0, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 1.5, 4}, .3, (color_t){1, 1, 1}, .16, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 1.5, 4}, .3, (color_t){1, 1, 1}, .33, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1.5, 4}, .3, (color_t){1, 1, 1}, .5, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 1.5, 4}, .3, (color_t){1, 1, 1}, .66, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 1.5, 4}, .3, (color_t){1, 1, 1}, .83, 1)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 1.5, 4}, .3, (color_t){1, 1, 1}, 1, 1)));

    assert(esferas.n == 23);
    for(size_t i = 0; i < esferas.n; i++)
        assert(esferas.v[i] != NULL);



    printf("P3\n");
    printf("%d %d\n", ANCHO, ALTO);
    printf("255\n");

    float vz = ANCHO / 2 / tan(FOV/ 2 * PI / 180);

    for(int vy = ALTO / 2; vy > - ALTO / 2; vy--)
        for(int vx = - ANCHO / 2; vx < ANCHO / 2; vx++) {
            color_imprimir(computar_intensidad(&esferas, &luces, ambiente, (vector_t){0, 0, 0}, vector_normalizar((vector_t){vx, vy, vz})));
            putchar('\n');
        }


    for(size_t i = 0; i < esferas.n; i++)
        esfera_destruir(esferas.v[i]);
    free(esferas.v);

    for(size_t i = 0; i < luces.n; i++)
        luz_destruir(luces.v[i]);
    free(luces.v);

    return 0;
}

// ###################################################