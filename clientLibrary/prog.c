#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include "clientLibrary.h"
#include "../datastructures/dsstring.h"

//  gcc -o prog prog.c clientLibrary.c ../datastructures/dsstring.c ../datastructures/dschunk.c
// ./prog < comandos.txt

int main(int argc , char *argv[]){
	int val;
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
    printf("\nsocket: %d",sock);
    key = dsStringNew();
    value = dsStringNew();
	char* command = (char*)malloc(sizeof(char)*(MAX_COMMAND_LENGTH+ 1));

	while(1){
		printf("\nCommand$: ");
	    int input = cl_inputString(stdin,command, key, value);
	    if(input != SUCCESS){
	    	cl_printError(input);
	    	dsStringDelete(&key);
	        dsStringDelete(&value);
	        
	        key = dsStringNew();
	        value = dsStringNew();
	    	continue;
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
	    if(!dsStringEmpty(key)){
	    	printf("key: ");
		    dsStringPrint(key);
	    }
	    reciveResponse(command,sock);
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



