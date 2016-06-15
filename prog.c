#include <stdio.h>
#include "clientLibrary/client.h"

//gcc -o prog prog.c clientLibrary/client.c


int main(){
	char str[] = " get       CLAVE1       valor3";
    int valor = validateInput(str);
    printf("el valor: %d", valor);
    return 0;
}