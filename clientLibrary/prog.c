#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "clientLibrary.h"
#include "dsstring.h"

//  gcc -o prog prog.c clientLibrary.c dsstring.c dschunk.c
// ./prog < comandos.txt

int main(int argc , char *argv[]){
	int i;
    dsString* key;
    dsString* value;

    key = dsStringNew();
    value = dsStringNew();

	char* command = (char*)malloc(sizeof(char)*(MAX_COMMAND_LENGTH+ 1));

	while(1){
		printf("Command$: ");
	    int input = cl_inputString(stdin,command, key, value);
	    if(input != SUCCESS){
	    	cl_printError(input);
	    	continue;
	    }
	    cl_exec(command, key, value);

	    ///imprime
	    if(!dsStringEmpty(key)){
	    	printf("key\n");
		    dsChunk* it;
			for(it = key->header; it!=NULL ; it= it->next ){
				printf("%s", it->cont);
			}
			printf("\n");
	    }

		if(!dsStringEmpty(value)){
			printf("value\n");
			dsChunk* it;
			for(it = value->header; it!=NULL ; it= it->next ){
				printf("%s", it->cont);
			} 
			printf("\n");
		} 	

	    
	    dsStringDelete(&key);
        dsStringDelete(&value);
        
        key = dsStringNew();
        value = dsStringNew();
    }

    return 0;
}



