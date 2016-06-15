#include <stdio.h>
#include <string.h>
#include "client.h"

void toUpper(char* string){

	char * it = string;
	while(*it!= '\0' ){
		if(*it >=97 && *it <= 122){
			*it = *it - 32;
		}
		it++;
	}
}


int validateInput(char* input){
    
    //se guarda el comando
    char comando[5];
	//recorrer y separar por espacios
	char * token;
	token = strtok(input," "); // el primer token

	//el primer token es un comando
	//los comandos son menores que 5
	if(strlen(token)< 5){
		strcpy(comando, token);
		toUpper(comando);
		printf ("comando:%s\n",comando);
	}
	else{
		//retorna error, comando muy grande
		return -1;
	}
	
	while (token != NULL){
		printf ("%s, %lu\n",token, strlen(token));
		token = strtok (NULL, " ");
	 }
	 
	 

    return 1;
}