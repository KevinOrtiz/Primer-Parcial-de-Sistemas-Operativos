
#include "Hash.h"
#include <assert.h>
#include "bstrlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../datastructures/dsstring.h"

static uint32_t Hashmap_djb2_hash(void *data);
// gcc -o programa principal.c Hash.c bstrlib.c darray.c ../../datastructures/dsstring.c ../../datastructures/dschunk.c

int main(){
    int respuesta;
    Hashmap *map = NULL;
    dsString* key1, *key2;
    dsString* value1, *value2;
    dsString* aux, *aux2;
    dsChunk* it;
    map = Hashmap_create(dsStringCmp, Hashmap_djb2_hash);
 

    key1 = dsStringNew(); //clave 1
    value1 = dsStringNew();//valor 1
    key2 = dsStringNew();//clave 2
    value2 = dsStringNew();//vlaor 2

//inicializa variables
    dsStringAdd(key1 , "MARISOL");
    dsStringAdd(key1 , "VILLACRESES");
    dsStringAdd(value1 , "COMPUTACION_Y_");
    dsStringAdd(value1 , "SOCIEDAD");
    dsStringAdd(key2 , "DR_MASTER_CARLOS_");
    dsStringAdd(key2 , "VALDIVIEZO");
    dsStringAdd(value2 , "MICROCONTROLADORES");

/*********************** SET ************************/       
    //ingreso la clave 1 y el valor 1 en el HASH
    respuesta = Hashmap_set(map, key1, value1);

    if(respuesta <0){
        printf ("Error al insertar\n");
        return 0; 
    }

    //ingreso la clave y el valor en el HASH
    respuesta = Hashmap_set(map, key2, value2);
    if(respuesta < 0){
        printf("Error al insertar 2\n");
        return 0;
    }

/*********************** GET ************************/

    aux = (dsString *) Hashmap_get(map, key1);
    if(aux){
        printf("Valor 1\n");
        dsStringPrint(aux); 
    }
    aux2 = (dsString *) Hashmap_get(map, key2);
    
    if(aux2){
        printf("Valor 2\n");
        dsStringPrint(aux2);
    }


/*********************** DEL ************************/
  aux = Hashmap_delete(map,key1);
   if(aux){
        printf("Eliminado clave 1: ");
        dsStringPrint(aux);

   }

    aux2 = Hashmap_delete(map,key2);
    if(aux2){
        printf("Eliminado clave 2: ");
        dsStringPrint(aux2);
    }


    printf("\nProbando otras cosas\n");
    aux = (dsString *) Hashmap_get(map, key1);
    if(aux){
        dsStringPrint(aux);    
    }
    aux2 = (dsString *) Hashmap_get(map, key2);
    if(aux2){
        dsStringPrint(aux2);
    }


    return 0;


}

static uint32_t Hashmap_djb2_hash(void *data)
{

    dsString* s = (dsString*)data;
    unsigned long hash = 5381;

    dsChunk* it;

    for(it=s->header; it !=NULL; it=it->next){
        char * str = it->cont;

        while (*str!= '\0'){
            hash = ((hash << 5) + hash) + *str;
            str++;
        }     

    }

    return hash;
}
