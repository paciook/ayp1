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
    float ka, kd, ks, kr;

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
    return (vector_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vector_t vector_interpolar_recta(vector_t o, vector_t d, float t){
    return (vector_t){o.x + (d.x)*t, o.y + (d.y)*t, o.z + (d.z)*t};
}

vector_t vector_normalizar(vector_t a){
    float m = vector_norma(a);
    return (vector_t){a.x/m, a.y/m, a.z/m};
}

vector_t vector_estirar(vector_t a, float factor){
    a.x *= factor;
    a.y *= factor;
    a.z *= factor;
    return a;
}

color_t color_sumar(color_t c, color_t m, float p){
    return (color_t){c.r + p*(m.r), c.g + p*(m.g), c.b + p*(m.b)};
}

color_t color_absorber(color_t b, color_t c){
    return (color_t){b.r*c.r, b.g*c.g, b.b*c.b};
}

int ftoppm(float f){
    return ((f > 1) ? 1 :f) * II;
}

void color_imprimir(color_t c){
    printf("%d %d %d ", ftoppm(c.r),ftoppm(c.g),ftoppm(c.b));
}

esfera_t *esfera_crear(vector_t centro, float radio, color_t color,
                        float ka, float kd, float ks, float kr){
    esfera_t *e = malloc(sizeof(esfera_t));
    if(e == NULL)
        return NULL;

    e->centro = centro;
    e->radio = radio;
    e->color = color;
    e->ka = ka;
    e->kd = kd;
    e->ks = ks;
    e->kr = kr;

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

    if(t <= 0){
        return INFINITO;
    }

    if(punto != NULL)
        *punto = vector_interpolar_recta(o,d,t);
    if(normal != NULL)
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

color_t computar_intensidad(int prof, const arreglo_t *esferas, const arreglo_t *luces,
                            color_t ambiente, vector_t o, vector_t d, size_t esf){
    color_t c = {0,0,0};
    if(prof <= 0) return c;

    vector_t p,n;
    float t,ant;
    t=ant=INFINITO;

    size_t n_esf = -1;

    for(size_t i = 0; i < esferas->n; i++){
        if(i == esf) continue;
        ant = esfera_distancia((esferas->v)[i], o, d, NULL, NULL);
        if(ant < t){
            t = ant;
            n_esf = i;
        }
    }

    if(t == INFINITO){
        return c;
    }
    assert(n_esf != -1 && t>=0);
    esfera_distancia((esferas->v)[n_esf], o, d, &p, &n);
    vector_t r = vector_estirar(n, 2*vector_producto_interno(d,n));
    r = vector_resta(d, r);

    esfera_t *esfera = (esfera_t*)((esferas->v)[n_esf]);

    for(size_t i = 0; i < luces->n; i++){
        luz_t *luz = (luz_t*)((luces->v)[i]);
        
        bool sigma = true;
        float dist_luz;
        vector_t l_luz;
        if(luz->es_puntual){
            vector_t p_to_l = vector_resta(luz->posicion,p);
            l_luz = vector_normalizar(p_to_l);
            dist_luz = vector_norma(p_to_l);
        } else {
            l_luz = luz->posicion;
            dist_luz = INFINITO;
        }

        for(size_t j = 0; j < esferas->n && sigma; j++){
            if(j == n_esf)
                continue;
            sigma = !(esfera_distancia(esferas->v[j],p,l_luz,NULL,NULL) < dist_luz);
        }

        if(sigma){
            /*
            color_t inten_i = color_absorber(luz->color,esfera->color);
            float prod_ln = vector_producto_interno(l_luz,n);
            c = color_sumar(c, inten_i, esfera->kd *  (prod_ln >= 0 ? prod_ln : 0));
            */
            float factor_angulo;

            // Calculo factor de angulo
            {
                // Termino del angulo de la normal
                float prod_ln = vector_producto_interno(l_luz,n);
                float termino_normal = esfera->kd *  (prod_ln >= 0 ? prod_ln : 0);

                // Termino del angulo del rebote
                float prod_lr = vector_producto_interno(l_luz,r);
                float termino_rebote = esfera->ks * pow((prod_lr >= 0 ? prod_lr : 0), 10);

                factor_angulo = termino_normal + termino_rebote;
            }
            color_t abs = color_absorber(luz->color, esfera->color);

            // Sumo el color de la luz
            c = color_sumar(c, abs, factor_angulo);
        }
    }

    c = color_sumar(c, ambiente, esfera->ka);

    return color_sumar(c, computar_intensidad(prof-1, esferas, luces, ambiente, p, r, n_esf), esfera->kr);
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

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1, 2.4}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.4, 3}, 1, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, -.6, 3}, .3, (color_t){1, 0, 0}, 1, .8, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1.73, -.6, 2}, .3, (color_t){1, 1, 0}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, -.6, 1.26}, .3, (color_t){0, 1, 0}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, -.6, 1}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, -.6, 1.26}, .3, (color_t){0, 1, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1.73, -.6, 2}, .3, (color_t){0, 0, 1}, 1, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, -.6, 3}, .3, (color_t){1, 0, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 0, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .16, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .33, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .5, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .66, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, .83, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 2.5, 4.3}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-3, 1.5, 4}, .3, (color_t){1, 1, 1}, 0, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-2, 1.5, 4}, .3, (color_t){1, 1, 1}, .16, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){-1, 1.5, 4}, .3, (color_t){1, 1, 1}, .33, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){0, 1.5, 4}, .3, (color_t){1, 1, 1}, .5, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){1, 1.5, 4}, .3, (color_t){1, 1, 1}, .66, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){2, 1.5, 4}, .3, (color_t){1, 1, 1}, .83, 1, 0.16, .33)));
    assert(arreglo_agregar(&esferas, esfera_crear((vector_t){3, 1.5, 4}, .3, (color_t){1, 1, 1}, 1, 1, 0.16, .33)));

    assert(esferas.n == 23);
    for(size_t i = 0; i < esferas.n; i++)
        assert(esferas.v[i] != NULL);



    printf("P3\n");
    printf("%d %d\n", ANCHO, ALTO);
    printf("255\n");

    float vz = ANCHO / 2 / tan(FOV/ 2 * PI / 180);

    for(int vy = ALTO / 2; vy > - ALTO / 2; vy--)
        for(int vx = - ANCHO / 2; vx < ANCHO / 2; vx++) {
            color_imprimir(computar_intensidad(5,&esferas, &luces, ambiente, (vector_t){0, 0, 0}, vector_normalizar((vector_t){vx, vy, vz}),-1));
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
