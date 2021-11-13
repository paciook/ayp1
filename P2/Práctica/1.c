#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool leer_dato(uint8_t *registro, uint8_t *dato){
    if(!(*registro & 0x80)){
        *dato = (*registro >> 2) & 0xe;
        *registro = *registro & 0x0;
        return true;
    }
    return false;
}

int main(void){
    uint8_t r = 0x0;
    uint8_t d = 0x0;
    printf("%d: %d\n", leer_dato(&r,&d), d);
    return 0;
}
