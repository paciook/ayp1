#include <stdio.h>
#include <math.h>

#include <float.h>
#define INFINITO FLT_MAX

#define IA  5
#define II  255

#define FOV 90
#define PI 3.1415926535897932
#define ANCHO 640
#define ALTO 480

const float luz[3] = {0.2873478855663454, 0.9578262852211513, 0};

const float centros[][3] = {
    {-.4, .75, 1.55},
    {-.55, -.4, 2},
    {3, 1.5, 4},
    {3, 1.5, 5},
    {3, -1.5, 4},
    {3, -1.5, 5},
};

const float radios[] = {
    .3,
    .85,
    .4,
    .4,
    .4,
    .4,
};

float producto_interno(const float a[3], const float b[3]){
    float prod = 0;
    for(int i = 0; i < 3; i++)
        prod += a[i]*b[i];

    return prod;
}

float norma(const float a[3]){
    return sqrt(producto_interno(a,a));
}

void normalizar(float a[3]){
    float nor = norma(a);

    for(int i = 0; i < 3; i++)
        a[i] /= nor;

}

void resta(float r[3], const float a[3], const float b[3]){
    for(int i = 0; i < 3; i++){
        r[i] = a[i] - b[i];
    }
    return;
}

void interpolar_recta(float p[3], const float o[3], const float d[3], float t){
    /*
     * Definir el punto P que se encuentra a distancia t del observador,
     * en dirección d
     */

    for(int i = 0; i < 3; i++)
        p[i] = o[i] + d[i]*t;

}

float distancia_esfera( const float c[3], const float r, const float o[3],
                        const float d[3]){
    
    /*
     * Calculo la distancia entre el observador y la intersección con la esfera
     */
    float t = INFINITO;
    
	float disc = 0;

    float res[3];
    resta(res,c,o);
    float p_res = producto_interno(res,d);

	disc = (p_res*p_res) - producto_interno(res,res) + r*r;
    
	
    if(disc >= 0){
        float sq = sqrt(disc);
        t = p_res + ((p_res <= sq) ? sq : -sq);
    }

    return (t > 0) ? t : INFINITO;
}

void normal_esfera( float normal[3], const float c[3], float r,
                    const float p[3]){
    /*
     * Defino en normal[3] el versor normal a la esfera de centr c[3]
     * y radio r, en el punto p[3].
     */
    resta(normal,p,c);
    normalizar(normal);
}

int computar_intensidad(const float cs[][3], const float rs[], size_t n_esferas,
                        const float luz[3], const float o[3], const float d[3], int prof){
    int inten = IA;

    // Digamos que no intersectamos nada hasta que se demuestre lo contrario
    int esf_cercana = -1;
    float dist_cercana = INFINITO;

    // Necesitamos saber si hay alguna esfera más cercana, para calcular la que
    // tengamos inmediatamente adelante
    for( int i = 0; i < n_esferas; i++){
        // Calculo la distancia con el i-ésimo y lo comparo con esf_cercana
        float dist_i = distancia_esfera(cs[i],rs[i],o,d);
        if(dist_i < dist_cercana){
            esf_cercana = i;
            dist_cercana = dist_i;
        }
    }

    // Si no intersecto ninguna esfera deuelvo la intensidad ambiente
    if(esf_cercana == -1)
        return IA;

    // Tengamos en p la intersección
    float p[3];
    interpolar_recta(p,o,d,dist_cercana);

    // Y en n el versor normal
    float n[3];
    normal_esfera(n,cs[esf_cercana],rs[esf_cercana],p);

    for(int i = 0; i < n_esferas; i++){
        // Omito revisar la esfera sobre la que estoy parado
        if(i == esf_cercana)
            continue;

        // Calculo la distancia con respecto a cada esfera
        float d_esf_sombra = distancia_esfera(cs[i],rs[i],p,luz);

        // Si intersecto alguna en dirección hacia la luz, devuelvo IA
        if(d_esf_sombra < INFINITO)
            return IA;
    }

    inten = IA + (producto_interno(n,luz) * II);

    return (inten>0) ? ((inten < II) ? inten : II) : IA;
}

int main (void){

    printf("P2\n%d %d\n%d\n",ANCHO,ALTO,II); 

    float z = ANCHO / (2 * (tan( ((FOV*PI) / 180) /2.0)));
    float o[3] = {0};

    for(float y = ALTO/2.0 - 1; y >= -ALTO/2.0; y--){
        for(float x = -ANCHO/2.0; x < ANCHO/2.0; x++){
            float d[3] = {x,y,z};
            normalizar(d);
            printf("%d ", computar_intensidad(centros, radios,sizeof(centros)/sizeof(centros[0]), luz,o,d));
        }
    }

    return 0;
}

