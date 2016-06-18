#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "clientLibrary/clientLibrary.h"

#define MAX 10
//gcc -o prog prog.c clientLibrary/client.c

void printHelp();
int inputString(FILE* fp);


int main(){
	int i,val=0;
	instruction parameters;
	int socket=-1;
	FILE *comando;
	//conectarse
	cl_connect();

	while(1){
		printf("Comand$: ");
	    val = inputString(stdin);
	    if(!val){
	    	printf("Error Fatal: no se pudo leer el comando\n");
	    	continue;
	    }
	    comando=fopen("temp","r");
	    if(comando && fgetc(comando)==EOF){
	    	//MEJORAR SI LA CONECCION AUN NO EXISTE CAMBIAR EL MENSAJE
	    	printf("Error Fatal: sin comando, se cierra la conexion\n");
	    	break;
	    }
	    fclose(comando);
	    int valor = cl_validateInput(&parameters);
	    if(valor==1)
	    	callMethod(socket,&parameters);
	    else
	    	printf("Error Fatal: el comando es incorrecto use el comando HELP\n");
	    
	}

    return 0;
}

void printHelp(){
	printf("Comands:\n");
	printf("\tget <key>\n");
	printf("\tset <key> <value>\n");
	printf("\tlist\n");
	printf("\tdel <key>\n");
	printf("\texit\n");
	printf("\thelp\n");
}

int inputString(FILE* fp){
//The size is extended by the input with the value of the provisional 
    if(!fp){
    	return NULL;
    }
    char str[MAX];
    int ch;
    size_t len = 0;
    FILE *temp;
    temp=fopen("temp","w");
    if(!temp)
    	return 0;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){  
        str[len++]=ch;
        if(len==(MAX-1)){
        	str[len]='\0';
        	fprintf(temp, "%s",str);
        	len=0;
        }
    }
    if(len!=0){
    	str[len]='\0';
    	fprintf(temp, "%s",str);
    }
    fclose(temp);
	return 1;
}