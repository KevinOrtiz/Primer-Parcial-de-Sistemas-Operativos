#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include "clientLibrary.h"
#include "dsstring.h"

//  gcc -o prog prog.c clientLibrary.c dsstring.c dschunk.c
// ./prog < comandos.txt

int main(int argc , char *argv[]){
	int val;
    dsString* key;
    dsString* value;
    int sock;

    sock=cl_connect("127.0.0.1","9999");
    printf("\nsocket: %d",sock);
    key = dsStringNew();
    value = dsStringNew();
	char* command = (char*)malloc(sizeof(char)*(MAX_COMMAND_LENGTH+ 1));

	while(1){
		printf("\nCommand$: ");
	    int input = cl_inputString(stdin,command, key, value);
	    if(input != SUCCESS){
	    	cl_printError(input);
	    	continue;
	    }
	    val=cl_exec(sock,command, key, value);
	    if(val==-1){
	    	printf("\nERROR: No se pudo ejecutar el comando");
	    	continue;
	    }
	    ///imprime
	    if(!dsStringEmpty(key)){
	    	printf("key: ");
		    dsStringPrint(key);
	    }

		if(!dsStringEmpty(value)){
			printf("value: ");
			dsStringPrint(value);
		} 	

	    
	    dsStringDelete(&key);
        dsStringDelete(&value);
        
        key = dsStringNew();
        value = dsStringNew();
    }

    return 0;
}



