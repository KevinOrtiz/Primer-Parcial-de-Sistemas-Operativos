#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include "clientLibrary.h"
#include "dsstring.h"

//  gcc -o cliente cliente.c clientLibrary.c dsstring.c dschunk.c
// ./cliente < comandos.txt

int main(int argc , char *argv[]){
	int val,val2;
    dsString* key;
    dsString* value;
    int sock;

    if(argc <= 2){
    	printf("Error: Faltan argumentos\n");
    	return 0;
    }


    sock=cl_connect(argv[1], argv[2]);
    if(!sock){
    	return 0;
    }
    key = dsStringNew();
    if(!key){
    	printf("No hay suficiente memoria en su sistema, se cierra");
    	close(sock);
    	return 0;
    }
    value = dsStringNew();
    if(!value){
    	printf("No hay suficiente memoria en su sistema, se cierra");
    	close(sock);
    	return 0;
    }
	char* command = (char*)malloc(sizeof(char)*(MAX_COMMAND_LENGTH+ 1));
	if(!command){
		printf("No hay suficiente memoria en su sistema, se cierra");
		close(sock);
    	return 0;
	}

	while(1){
		printf("\nCommand$: ");
	    int input = cl_inputString(stdin,command, key, value);
	    if(input != SUCCESS){
	    	cl_printError(input);
	    	dsStringDelete(&key);
	        dsStringDelete(&value);
	        
	        key = dsStringNew();
	        value = dsStringNew();

	        if(!key){
	        	continue;
	        }
	        if(!value){
	        	continue;
	        }

	    	
	    }
	    val=cl_exec(sock,command, key, value);
	    if(val==-1){
	    	printf("\nERROR: No se pudo ejecutar el comando");
	    	dsStringDelete(&key);
	        dsStringDelete(&value);
	        
	        key = dsStringNew();
	        value = dsStringNew();
	    	continue;
	    }
	    ///imprime
	    if(!dsStringEmpty(key) && !strcmp(command,"GET")){
		    dsStringPrint(key);
		    printf("=");
	    }
	    val2=reciveResponse(command,sock);
	    if(val2==-1){
	    	printf("ERROR: no se pudo recibir la respuesta\n");
	    	dsStringDelete(&key);
	        dsStringDelete(&value);
	        
	        key = dsStringNew();
	        value = dsStringNew();
	    	continue;
	    }
	    if(!strcmp(command,"SET") || !strcmp(command,"DEL"))
	    	printf("OK");

	    /*
		if(!dsStringEmpty(value)){
			printf("value: ");
			dsStringPrint(value);
		} 
		*/	

	    
	    dsStringDelete(&key);
        dsStringDelete(&value);
        
        key = dsStringNew();
        value = dsStringNew();
    }

    return 0;
}



