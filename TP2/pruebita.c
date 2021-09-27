#include <stdio.h>

void printear(const float a[3]){
    for(int i = 0; i < 3; i++){
        printf("%f", a[i]);
    }
    return;
}

void resta(float r[3], const float a[3], const float b[3]){
    for(int i = 0; i < 3; i++){
        printf("%d %f\n",i, a[i]);
        r[i] = a[i] - b[i];
    }
    return;
}

int main(void){
    float a[3] = {1,2,3};
    float b[3] = {1,2,3};
    printear(a);
    float r[3] = {0};
    resta(r,a,b);
    printear(r);
    return 0;
}