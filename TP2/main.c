#include <stdio.h>
#include <math.h>

#include <float.h>
#define INFINITO FLT_MAX

#define IMAX 255

#define IA  5
#define II  255

#define FOV 90
#define PI 3.14159
#define ANCHO 640
#define ALTO 480


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

    return;
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

    return;
}

float distancia_esfera( const float c[3], const float r, const float o[3],
                        const float d[3]){
    
    /*
     * Calculo la distancia entre el observador y la intersección con la esfera
     */
    float t = INFINITO;
    
	float disc = 0;

    float res[3];
    resta(res,o,c);

	disc = pow((-producto_interno(o,d)+producto_interno(c,d)),2)-
                (producto_interno(res,res)) + pow(r,2);

	if(disc >= 0){
		t = -producto_interno(o,d) + producto_interno(c,d) - sqrt(disc);
        if(t > 0)
            return t;
    }
    return INFINITO;
}

void normal_esfera( float normal[3], const float c[3], float r,
                    const float p[3]){
    /*
     * Defino en normal[3] el versor normal a la esfera de centr c[3]
     * y radio r, en el punto p[3].
     */
    
    for(int i = 0; i<3;i++)
        normal[i] = (p[i] - c[i]) / r;

    return;
}

int computar_intensidad(const float cs[][3], const float rs[], size_t n_esferas,
                        const float luz[3], const float o[3], const float d[3]){
    int inten = IA;

    // Digamos que no intersectamos nada hasta que se demuestre lo contrario
    int esf_cercana = -1;
    float dist_c = INFINITO;

    // Necesitamos saber si hay alguna esfera más cercana, para calcular la que
    // tengamos inmediatamente adelante
    for( int i = 0; i < n_esferas; i++){
        // Calculo la distancia con el i-ésimo y lo comparo con el más esf_cercana
        float dist_i = distancia_esfera(cs[i],rs[i],o,d);
        if(dist_i < dist_c){
            esf_cercana = i;
            dist_c = dist_i;
        }
    }

    // Si no intersecto ninguna esfera deuelvo la intensidad ambiente
    if(esf_cercana == -1)
        return IA;

    // Tengamos en p la intersección
    float p[3];
    interpolar_recta(p,o,d,dist_c);

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

    // Calculo la intensidad en escala desde 0 a 255
    inten = producto_interno(n,luz) * IMAX;

    // Si la intensidad es negativa, devuelvo 0.
    if (inten < 0)
        return 0;

    return inten;
}

int main (void){

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

    printf("P2\n%d %d\n%d\n",ANCHO,ALTO,IMAX); 

    float z = ANCHO / (2 * (tan( ((FOV*PI) / 180) /2.0)));
    float o[3] = {0};

    for(int y = ALTO/2 - 1; y >= -ALTO/2; y--){
        for(int x = -ANCHO/2; x < ANCHO/2; x++){
            float d[3] = {x,y,z};
            normalizar(d);
            printf("%d ", computar_intensidad(centros, radios,sizeof(centros)/sizeof(centros[0]), luz,o,d));
        }
    }

    return 0;
}

