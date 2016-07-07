#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include "clientLibrary.h"
#include "../datastructures/dsstring.h"


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
        
        while(EOF!=ch && ch!='\n'){
            ch=fgetc(fp);
        };
        
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
        i = 1;

        while(EOF!=(ch=fgetc(fp)) && ch!='\n' && ch!=' '){
            if(i > MAX_KEY_LENGTH){
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
        i = 1;
        while(EOF!=(ch=fgetc(fp)) && ch!='\n' && ch!=' '){
            if(i > MAX_VALUE_LENGTH){
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
        //printf("El comando es GET\n");
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"SET")==0){
        //printf("El comando es SET\n");
        return 2; //necesitan dos paramentros
    }
    if(strcmp(command,"LIST")==0){
        //printf("El comando es LIST\n");
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command,"DEL")==0){
        //printf("El comando es DEL\n");
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"EXIT")==0){
        //printf("El comando es EXIT\n");
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command, "HELP")==0){
        //printf("El comando es HELP\n");
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
int sendKey(int sock,dsString* key){
    return dsStringSendChunkSocket(key,sock);
}

int sendKeyValue(int sock,dsString* key,dsString* value){
    int ban;
    ban=dsStringSendChunkSocket(key,sock);
    if(ban!=SUCCESS)
        return ban;
    return dsStringSendChunkSocket(value,sock);
}

int cl_exec(int sock,char* command, dsString* key, dsString* value){

    if(strcmp(command, "HELP")==0){
        cl_help();
        return SUCCESS;
    }
    if(strcmp(command,"GET")==0){
        printf("se ejecuta GET\n");
        return cl_get(sock,key);
    }
    if(strcmp(command,"SET")==0){
        printf("se ejecuta SET\n");
        return cl_set(sock,key,value);
    }
    if(strcmp(command,"LIST")==0){
        printf("se ejecuta LIST\n");
        return cl_list(sock); 
    }
    if(strcmp(command,"DEL")==0){
        printf("se ejecuta DEL\n");
        cl_del(sock,key);
        return SUCCESS;
    }
    if(strcmp(command,"EXIT")==0){
        printf("se ejecuta EXIT\n");
        return cl_disconnect(sock); 
    }
    return -1; 
}

/*
 * Esta funcion retorna un entero que me indica si el proceso para crear el socket fue exitoso
 * Retorna -1: error de creacion del socket
 * Retorna  0: error de conexion
 * Retorna > 0: el socket se creo con exito
 */
int cl_connect(char* ip, char* puerto){
    int sock;
    struct sockaddr_in server;
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock < 0)
        return -1; //printf("ERROR al crear el socket\n");

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(puerto) );
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        return 0; //fallo la conexion

    char command[20];
    int read_size = recv(sock , command , 20, 0);
    if(read_size>0){
        command[read_size]='\0';    
    } 
    else{
        close(sock);
        return 0; 
    } 
    if(strcmp(command, "CONECTION_OK")==0){
        return sock;
    }
    if(strcmp(command, "CONECTION_NK")==0){
        printf("Error: servidor ocupado\n");
        close(sock);
        return 0;
    }
    return 0;
}

int cl_get(int socket,dsString* key){
    char server_reply[10];
    int read_size;

    if( send(socket , "GET", strlen("GET") , 0) < 0) 
        return -1;
    read_size = recv(socket , server_reply , 10 , 0);
    server_reply[read_size]='\0';
    if(strcmp(server_reply,"OK"))
        return -1;
    printf("Envie el Comando a el servidor: GET\n");
    return sendKey(socket,key);
}

int cl_set(int socket,dsString* key,dsString* value){
    char server_reply[10];
    int read_size;
    
    if( send(socket , "SET", strlen("SET") , 0) < 0) 
        return -1;
    read_size = recv(socket , server_reply , 10 , 0);
    server_reply[read_size]='\0';
    if(strcmp(server_reply,"OK"))
        return -1;
    printf("Envie el Comando a el servidor: SET\n");
    return sendKeyValue(socket,key,value);
}

int cl_list(int socket){
    char server_reply[10];
    int read_size;
    
    if( send(socket , "LIST", strlen("LIST") , 0) < 0) 
        return -1;
    read_size = recv(socket , server_reply , 10 , 0);
    server_reply[read_size]='\0';
    if(strcmp(server_reply,"OK"))
        return -1;
    printf("Envie el Comando a el servidor: LIST\n");
   
    return SUCCESS;
}


int cl_del(int socket,dsString* key){
    char server_reply[10];
    int read_size;
    
    if( send(socket , "DEL", strlen("DEL") , 0) < 0) 
        return SUCCESS;
    read_size = recv(socket , server_reply , 10 , 0);
    server_reply[read_size]='\0';
    if(strcmp(server_reply,"OK"))
        return -1;
    printf("Envie el Comando a el servidor: DEL\n");
    return sendKey(socket,key);

}

void cl_help(){
    printf("Comands:\n");
    printf("\tget <key>\n");
    printf("\tset <key> <value>\n");
    printf("\tlist\n");
    printf("\tdel <key>\n");
    printf("\texit\n");
    printf("\thelp\n");
}


//*******MODIFICAR EXIT*****///
int cl_disconnect(int socket){
	char server_reply[10];
    int read_size;

    if( send(socket , "EXIT", strlen("EXIT") , 0) < 0) 
        return -1;
    read_size = recv(socket , server_reply , 10 , 0);
    server_reply[read_size]='\0';
	
    //int valor ;
    /*
    int shutdiwn(socket,how)
    Cierra la conexion del socket <socket> , The argument how especifica las siguientes acciones:
    0 Stop to receiving data del socket. Si algun dato arriva al socket lo elimina
    1 Stop de enviar datos al socket, todos los datos esperando por ser enviador se descartan
    2 Detiene tanto la recepcion como la transmision

    */
    /*
    valor = shutdown(socket,2);
    if (valor < 0){
        printf("No se cerro correctamente el socket \n");

    }
    printf("socket correctamente cerrado \n");
    */
    exit(0);

}
