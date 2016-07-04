
#include "Hash.h"
#include <assert.h>
#include "bstrlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t* Hashmap_fnv1a_hash(void *data);

int main(){
    int numero = 1234;
    char dato[50] = {'\0'}, *resultado;
    Hashmap *map = NULL;
    uint32_t hash = 0;

    strcpy(dato,"HOLA MUNDO");
    map = Hashmap_create(NULL, NULL);
    Hashmap_set(map, &numero, dato);

    memset(dato,'\0',strlen(dato));
    strcpy(dato,"QUE TAL");
    numero++;
    Hashmap_set(map, &numero, dato);

    memset(dato,'\0',strlen(dato));
    strcpy(dato,"HOLA DE NUEVO");
    numero--;
    Hashmap_set(map, &numero, dato);

    numero = 1234;
    resultado = (char *)Hashmap_get(map,&numero);
    printf("%s\n", resultado);

    return 0;


}

uint32_t* Hashmap_fnv1a_hash(void *data)
{
    bstring s = (bstring)data;
    uint32_t hash = FNV_OFFSET_BASIS;
    int i = 0;

    for(i = 0; i < blength(s); i++) {
        hash ^= bchare(s, i, 0);
        hash *= FNV_PRIME;
    }

    return hash;
}
