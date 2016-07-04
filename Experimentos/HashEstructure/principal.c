
#include "Hash.h"
#include <assert.h>
#include "bstrlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../datastructures/dsstring.h"

static uint32_t Hashmap_fnv1a_hash(void *data);
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;
// gcc -o programa principal.c Hash.c bstrlib.c darray.c ../../datastructures/dsstring.c ../../datastructures/dschunk.c

int main(){
    int respuesta;
    Hashmap *map = NULL;
    dsString* key1, *key2;
    dsString* value1, *value2;
    dsString* aux, *aux2;
    dsChunk* it;

    map = Hashmap_create(dsStringCmp, Hashmap_fnv1a_hash);

    key1 = dsStringNew();
    value1 = dsStringNew();
/*********************** SET ************************/
    dsStringAdd(key1 , "12345");
    dsStringAdd(value1 , "PRUEBA1");
    //ingreso la clave y el valor en el HASH
    Hashmap_set(map, key1, value1);

    key2 = dsStringNew();
    value2 = dsStringNew();

    dsStringAdd(key2 , "99999");
    dsStringAdd(value2 , "PRUEBA2");
    //ingreso la clave y el valor en el HASH
    Hashmap_set(map, key2, value2);
/*********************** GET ************************/
    aux = (dsString *) Hashmap_get(map, key1);
    dsStringPrint(aux);

    aux2 = (dsString *) Hashmap_get(map, key2);
    dsStringPrint(aux2);
/*********************** DEL ************************/
    aux = Hashmap_delete(map,key1);

    aux2 = Hashmap_delete(map,key2);

    /*dsStringPrint(aux);
    dsStringPrint(aux2);

    aux = (dsString *) Hashmap_get(map, key1);
    dsStringPrint(aux);

    aux2 = (dsString *) Hashmap_get(map, key2);
    dsStringPrint(aux2);*/

    /*dsStringDelete(&key1);
    dsStringDelete(&value1);
    dsStringDelete(&key2);
    dsStringDelete(&value2);*/

    return 0;


}

static uint32_t Hashmap_fnv1a_hash(void *data)
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
