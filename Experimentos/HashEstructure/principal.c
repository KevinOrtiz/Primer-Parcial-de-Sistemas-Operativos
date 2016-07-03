
#include "Hash.h"
#include <assert.h>
#include "bstrlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//No se recomienda declarar esta estructura en los archivos .C, hacerlo en los .h
int main(){
    int numero = 1234;
    HashmapNode *map = NULL;
    uint32_t hash = 0;
    char *nombre = "HOLA A TODOS";
    //carro *bus = (carro *)malloc(sizeof(carro));
    //strcpy(bus->nombre,"HOLA A TODOS");
    //bus->placa = numero;
    map = Hashmap_node_create(hash,numero,nombre);
    //HashmapNode *nodo = (HashmapNode *)malloc(sizeof(HashmapNode));
    //*(int *)map->key = bus->placa;
    //*(char *)map->data = bus->nombre;
    //printf("%d", bus->placa);
    //printf("%s", bus->nombre);
    printf("%d\n", map->key);
    printf("%s\n", map->data);
    return 0;


}
