#include <stdio.h>
#include "clientLibrary/client.h"

//gcc -o prog prog.c clientLibrary/client.c


int main(){
    int valor = validateInput(NULL);
    printf("el valor: %d", valor);
    return 0;
}