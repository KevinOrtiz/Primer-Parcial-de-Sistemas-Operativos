#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "clientLibrary.h"

/*
typedef struct instruction {
	char command[5];
	char key[MAXKEY];
	FILE* value;
}instruction;
*/
void printHelp(){
	printf("Comands:\n");
	printf("\tget <key>\n");
	printf("\tset <key> <value>\n");
	printf("\tlist\n");
	printf("\tdel <key>\n");
	printf("\texit\n");
	printf("\thelp\n");
}

void toUpper(char* string){

	while(*string!= '\0' ){
		if(*string >=97 && *string <= 122){
			*string = *string - 32;
		}
		string++;
	}
}

int onlyCommand(char *command){
	return !strcmp(command,"EXIT") || !strcmp(command,"HELP") || !strcmp(command,"LIST");
}

int onlykey(char *command){
	return !strcmp(command,"GET") || !strcmp(command,"DEL");
}

//retorna un codigo que indica error o exito
// -1 : error por falta de memoria
// 1 : exito
// -2: Error de parsing comando muy grande, mayor que 5 caracteres,numero incorrecto de parametros
//key 128MB

int cl_validateInput(instruction* parameters){

    //inicializo el arreglo de paremetros
    int i =0;
    char c;
    FILE *temp;

    temp=fopen("temp","r");
    //se obtiene el comando
    while(EOF != (c=fgetc(temp)) && c!=' '){
    	(parameters->command)[i++]=c;
    	if(i==5) // si el comando supera el maximo de caracteres
    		return -2;
    }
    (parameters->command)[i]='\0';
    toUpper(parameters->command);
    while(EOF != (c=fgetc(temp)) && c==' ');//quitar espacion en blanco

    if(onlyCommand(parameters->command) && c==EOF)// es instruccion es sin parametros
    	return 1;
    if(onlyCommand(parameters->command))//numero incorrecto de parametros
    	return -2;
    if(c==EOF)//numero incorrecto de parametros
    	return -2;
    //se obtiene el key
    i=0;
    (parameters->key)[i++]=c;
    while(EOF != (c=fgetc(temp)) && c!=' '){
    	(parameters->key)[i++]=c;
    	if(i==MAXKEY) // si la clave supera el maximo de caracteres
    		return -2;
    }
    (parameters->key)[i]='\0';
    while(EOF != (c=fgetc(temp)) && c==' ');//quitar espacion en blanco

    if(onlykey(parameters->command) && c==EOF)// es instruccion de 1 solo parametro
    	return 1;
    if(onlykey(parameters->command))//numero incorrecto de parametros
    	return -2;
    if(c==EOF)//numero incorrecto de parametros
    	return -2;
    fseek(temp,-1*sizeof(char),SEEK_CUR);
    parameters->value=temp;
    return 1;
}

/*
 * Esta funcion retorna un entero que me indica si el proceso para crear el socket fue exitoso
 * Retorna -1: error de creacion del socket
 * Retorna  0: error de conexion
 * Retorna > 0: el socket se creo con exito
 */
int cl_connect(char *argv[]){
    int sock;
    struct sockaddr_in server;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock < 0)
        return -1; //printf("ERROR al crear el socket\n");

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[2]) );
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        return 0; //fallo la conexion

	return sock;
}

char* cl_get(char* key){
	return NULL;
}
char* cl_set(char* key, FILE* value){
	printf("<");
	char c;
	while(EOF != (c=fgetc(value)) && c!=' ')
		printf("%c",c );
	printf(">\n" );
	return NULL;
}
char* cl_list(){
	return NULL;

}
char* cl_del(char* key){
	return NULL;
}

int callMethod(int socket,instruction* parameters){

	if(strcmp(parameters->command,"GET")==0){
		printf("Se ejecuta GET <%s>\n",parameters->key);
		cl_get(parameters->key);
		return 0;
	}
	if(strcmp(parameters->command,"SET")==0){
		printf("Se ejecuta SET <%s> ",parameters->key);
		cl_set(parameters->key, parameters->value);
		if(parameters->value)
	    	fclose(parameters->value);
		return 0;
	}
	if(strcmp(parameters->command,"LIST")==0){
		printf("Se ejecuta LIST\n");
		cl_list();
		return 0;
	}
	if(strcmp(parameters->command,"DEL")==0){
		printf("Se ejecuta DEL <%s>\n",parameters->key);
		cl_del(parameters->key);
		return 0;
	}
	if(strcmp(parameters->command,"EXIT")==0){
		printf("Se ejecuta EXIT\n");
		cl_disconnect(socket);
		exit(0);
	}
	if(strcmp(parameters->command,"HELP")==0){
		printHelp();
		return 0;
	}
	printf("Comando %s no reconocido\n", parameters->command);
	return 0;

}
//*******MODIFICAR EXIT*****///
void cl_disconnect(int socket){
	printf("Desconectar\n");
	close(socket);
}
