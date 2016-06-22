#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "clientLibrary/clientLibrary.h"

#define MAX 10
//desde la terminal escribe make prog


int inputString(FILE* fp);


int main(int argc , char *argv[]){
	int i,val=0;
	instruction parameters;
	int socket=-1;
	FILE *comando;
    //char message[1000] , server_reply[1000];
	socket = cl_connect(argv);
	if( socket < 0 )
        printf("ERROR al crear el socket...\n");
    puts("Conectado...");

	while(1){
        /*printf("Ingrese : ");
        bzero(message,1000);
        fgets(message,1000,stdin);

        //Send some data
        if( send(socket , message , strlen(message) , 0) < 0)
        {
            puts("Fallo el envio\n");
            return 1;
        }

        //Receive a reply from the server
        if( recv(socket , server_reply , 1000 , 0) < 0)
        {
            puts("recv failed\n");
            break;
        }

        printf("Respuesta del servidor : ");
        puts(server_reply);*/
		printf("Comand$: ");
	    val = inputString(stdin);
	    if(!val){
	    	printf("Error Fatal: no se pudo leer el comando\n");
	    	continue;
	    }
	    comando=fopen("temp","r");
	    if(comando && fgetc(comando)==EOF){
	    	//MEJORAR SI LA CONECCION AUN NO EXISTE CAMBIAR EL MENSAJE
	    	printf("Error Fatal: sin comando, se cierra la conexion\n");
	    	break;
	    }
	    fclose(comando);
	    int valor = cl_validateInput(&parameters);
	    if(valor==1)
	    	callMethod(socket,&parameters);
	    else
	    	printf("Error Fatal: el comando es incorrecto use el comando HELP\n");
	}

    return 0;
}


int inputString(FILE* fp){
//The size is extended by the input with the value of the provisional
    if(!fp){
    	return 0;
    }
    char str[MAX];
    int ch;
    size_t len = 0;
    FILE *temp;
    temp=fopen("temp","w");
    if( !temp )
    	return 0;
    while( EOF != (ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==(MAX-1)){
        	str[len]='\0';
        	fprintf(temp, "%s",str);
        	len=0;
        }
    }
    if(len!=0){
    	str[len]='\0';
    	fprintf(temp, "%s",str);
    }
    fclose(temp);
	return 1;
}
