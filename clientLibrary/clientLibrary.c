#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "clientLibrary.h"
#include "dsstring.h"


/*
typedef struct instruction {
	char command[5];
	char key[MAXKEY];
	FILE* value;
}instruction;
*/


void cl_printHelp(){
	printf("Comands:\n");
	printf("\tget <key>\n");
	printf("\tset <key> <value>\n");
	printf("\tlist\n");
	printf("\tdel <key>\n");
	printf("\texit\n");
	printf("\thelp\n");
}

void cl_toUpper(char* string){

    while(*string!= '\0' ){
        if(*string >=97 && *string <= 122){
            *string = *string - 32;
        }
        string++;
    }
}

int cl_inputString(FILE* fp, char* command, dsString* key, dsString* value){
    //validar argumentos
    if(!fp){
        return WRONG_ARGUMENT;
    }
    if(!command){
        return WRONG_ARGUMENT;
    }
    if(!key){
        return WRONG_ARGUMENT;
    }
    if(!value){
        return WRONG_ARGUMENT;
    }

    int ch; 
    long i =0;

    //quita espacios
    while(EOF!=(ch=fgetc(fp)) && ch!='\n'){
        if(ch!=' ') break;
    }
    //si llego al final y solo hay espacios, error de parsing
    if(ch =='\n' || ch == EOF) 
        return PARSE_ERROR;


    command[i]  = ch;
    i++;
    //recupera el comando
    //se leen los siguientes MAX_COMMAND_LENGTH caracteres
    //si se encuentran espacios se termina
    while(EOF!=(ch=fgetc(fp)) && ch!='\n' && ch!=' '){
        if(i == MAX_COMMAND_LENGTH){
            //limpiar buffer
            while(EOF!=(ch=fgetc(fp)) && ch!='\n'); 

            //error de parsin
            return PARSE_ERROR;
        }
        command[i] = ch;
        i++;
    }

    command[i] = '\0';
    cl_toUpper(command);
    int numArgs = cl_validateCommand(command);
    //comando no reconocido
    if(numArgs<0){
        //limpiar buffer
        while(EOF!=(ch=fgetc(fp)) && ch!='\n');
        
        return PARSE_ERROR;
    }
        
        

    if(numArgs==0){
        //limpiar el buffer
        while(EOF!=ch && ch!='\n'){
            ch=fgetc(fp);
        };  
        return SUCCESS;
    }

    if(numArgs > 0){
        //validar que hayan mas caracteres
        if(ch==EOF || ch == '\n')
            return LACK_OF_ARGUMENT;
        //quitar espacios
        while(EOF!=(ch=fgetc(fp)) && ch!='\n'){
            if(ch!=' ') break;
        }
        //si llego al final error
        if(ch==EOF || ch == '\n')
            return LACK_OF_ARGUMENT;
        //leer el primer argumento y ponerlo en Key
        dsStringAddChar(key, ch);
        int i = 1;

        while(EOF!=(ch=fgetc(fp)) && ch!='\n' && ch!=' '){
            if(i == MAX_KEY_LENGTH){
                //limpiar buffer      
                while(EOF!=ch && ch!='\n'){
                    ch=fgetc(fp);
                };  
                //error de parsin
                return BIG_KEY;
            }
            dsStringAddChar(key, ch);
            i++;
        }
        dsStringAddChar(key, '\0');
    }
    if(numArgs > 1){

        //validar que hayan mas caracteres
        if(ch==EOF || ch == '\n')
            return LACK_OF_ARGUMENT;
        //quitar espacios
        while(EOF!=(ch=fgetc(fp)) && ch!='\n'){
            if(ch!=' ') break;
        }
        //si llego al final error
        if(ch==EOF || ch == '\n')
            return LACK_OF_ARGUMENT;

        //leer el primer argumento y ponerlo en Key
        dsStringAddChar(value, ch);
        int i = 1;
        while(EOF!=(ch=fgetc(fp)) && ch!='\n' && ch!=' '){
            if(i == MAX_VALUE_LENGTH){
                //limpiar buffer
                while(EOF!=(ch=fgetc(fp)) && ch!='\n'); 

                //error de parsin
                return BIG_VALUE;
            }
            dsStringAddChar(value, ch);
            i++;
        }
        dsStringAddChar(value, '\0');
    }

    //limpia el buffer
    while(EOF!=ch && ch!='\n'){
        ch=fgetc(fp);
    };   

    return SUCCESS;
}

//valida si el comando es valido
//si no es valido retorna -1
//si es valido retorna el numero de argumentos que requiere
int cl_validateCommand(char* command){
    if(strcmp(command,"GET")==0){
        printf("El comando es GET\n");
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"SET")==0){
        printf("El comando es SET\n");
        return 2; //necesitan dos paramentros
    }
    if(strcmp(command,"LIST")==0){
        printf("El comando es LIST\n");
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command,"DEL")==0){
        printf("El comando es DEL\n");
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"EXIT")==0){
        printf("El comando es EXIT\n");
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command, "HELP")==0){
        printf("El comando es HELP\n");
        return 0; //no se necesitan paramentros
    }
    return -1; //error comando no reconocido

}

void cl_printError(int errorCode){
    switch(errorCode){
        case SUCCESS:
            printf("Sin error\n");
            break;
        case INSUFFICIENT_MEMORY:
            printf("Su sistema ya no tiene memoria\n");
            break;
        case PARSE_ERROR:
            printf("Error de parseo: el comando no existe\n");
            break;
        case WRONG_ARGUMENT:
            printf("La funcion recibio un mal argumento\n");
            break;
        case BIG_KEY:
            printf("La clave es mayor que %ld bytes\n", (long)MAX_KEY_LENGTH);
            break;
        case BIG_VALUE:
            printf("El valor es mayor que %ld bytes\n", (long)MAX_VALUE_LENGTH);
            break;
        case LACK_OF_ARGUMENT:
            printf("Al comando le faltan argumentos\n");
            break;
    }
}


int cl_exec(char* command, dsString* key, dsString* value){
    
    if(strcmp(command,"EXIT")==0){
        
        //cl_disconnect();
        exit(0);
    }
    if(strcmp(command, "HELP")==0){
        cl_printHelp();
        return SUCCESS;
    }
    return SUCCESS;
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

/*int callMethod(int socket,instruction* parameters){

	if(strcmp(parameters->command,"GET")==0){
		printf("Se ejecuta GET <%s>\n",parameters->key);
		cl_get(parameters->key);
		return 0;
	}
	if(strcmp(parameters->command,"SET")==0){
		printf("Se ejecuta SET <%s> ",parameters->key);
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

}*/
//*******MODIFICAR EXIT*****///
void cl_disconnect(int socket){
	printf("Desconectar\n");
	//close(socket);
}
