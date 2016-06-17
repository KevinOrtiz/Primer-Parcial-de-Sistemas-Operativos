#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "clientLibrary/client.h"

//gcc -o prog prog.c clientLibrary/client.c

void printHelp();
char *inputString(FILE* fp, size_t size);
int callMethod(char** parameters);

int main(){
	int i;
	char * input;
	char* parameters[3];

	//conectarse
	cl_connect();

	while(1){
		printf("$");
	    input = inputString(stdin, 30);
	    if(!input){
	    	printf("Error Fatal: falta de memoria para el mensaje\n");
	    	continue;
	    }
	    if(*input=='\0'){
	    	//MEJORAR SI LA CONECCION AUN NO EXISTE CAMBIAR EL MENSAJE
	    	printf("Error Fatal: sin comando, se cierra la conexion\n");
	    	break;
	    }
	    int valor = cl_validateInput(input, parameters, 3);
	    if(valor<0){
	    	//manejo de errores
	    	printf("Error\n");
	    }

	    callMethod(parameters);

	    free(input);	
	}

    return 0;
}

void printHelp(){
	printf("comands: get key\n");
	printf("          set key value\n");
	printf("          list\n");
	printf("          del key\n");
	printf("          exit\n");
	printf("          help\n");
}

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional 
    if(!fp){
    	return NULL;
    }
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;

    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){  
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
	str[len++]='\0';
	str = realloc(str, sizeof(char)*len);

    return str;
}

int callMethod(char** parameters){
	char* command = parameters[0];

	if(strcmp(command,"GET")==0){
		printf("Se ejecuta GET\n");
		cl_get(parameters[1]);
		return 0;
	}
	if(strcmp(command,"SET")==0){
		printf("Se ejecuta SET\n");
		cl_set(parameters[1], parameters[2]);
		return 0;
	}
	if(strcmp(command,"LIST")==0){
		printf("Se ejecuta LIST\n");
		cl_list();
		return 0;
	}
	if(strcmp(command,"DEL")==0){
		printf("Se ejecuta DEL\n");
		cl_del(parameters[1]);
		return 0;
	}
	if(strcmp(command,"EXIT")==0){
		printf("Se ejecuta EXIT\n");
		cl_disconnect();
		exit(0);
	}
	if(strcmp(command,"HELP")==0){
		printHelp();
		return 0;
	}
	printf("Comando %s no reconocido\n", command);
	return 0;

}