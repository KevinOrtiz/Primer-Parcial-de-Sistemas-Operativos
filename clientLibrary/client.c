#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

void toUpper(char* string){

	while(*string!= '\0' ){
		if(*string >=97 && *string <= 122){
			*string = *string - 32;
		}
		string++;
	}
}



//retorna un codigo que indica error o exito
// -1 : error por falta de memoria
// 1 : exito
// -2: Error de parsing comando muy grande, mayor que 5 caracteres


int cl_validateInput(char* input, char** parameters, int numParams){
    
    //inicializo el arreglo de paremetros
    int i =0;
	for(i=0; i< numParams; i++){
		parameters[i] = NULL;
	}

    //variable para guardar el comando
    char* comando;
    comando = (char*)malloc(sizeof(char)*5);
    if(!comando){
    	return -1;
    }
	//recorrer y separar por espacios
	char * token;
	token = strtok(input," "); // el primer token

	//el primer token es un comando
	//los comandos son menores que 5
	if(strlen(token)< 5){
		strcpy(comando, token);
		toUpper(comando);
		parameters[0] = comando;
	}
	else{
		//retorna error, comando muy grande
		return -2;
	}
	i = 1;
	while (i< numParams){
		token = strtok (NULL, " ");
		if(token){
			parameters[i] = token;
			i++;	
		}else{
			break;
		}

	 }

    return 1;
}


char* cl_connect(){
	return NULL;
}

char* cl_get(char* key){
	return NULL;
}
char* cl_set(char* key, char* value){
	return NULL;
}
char* cl_list(){
	return NULL;

}
char* cl_del(char* key){
	return NULL;
}

//*******MODIFICAR EXIT*****///
char* cl_disconnect(){
	return NULL;
}