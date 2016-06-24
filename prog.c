#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "clientLibrary/clientLibrary.h"
#include "list/list.h"


//gcc -o prog prog.c clientLibrary/clientLibrary.c list/nodelist.c list/list.c





int main(int argc , char *argv[]){
	int i;
    List* input;
	instruction parameters;
	int socket=-1;
	FILE *comando;
    //char message[1000] , server_reply[1000];
	//socket = cl_connect(argv);
	//if( socket < 0 )
    //    printf("ERROR al crear el socket...\n");
    //puts("Conectado...");

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
	    input = inputString(stdin);
	    if(!input){
	    	printf("Error Fatal: no se pudo leer el comando\n");
	    	continue;
	    }
        /*
	    if(comando && fgetc(comando)==EOF){
	    	//MEJORAR SI LA CONECCION AUN NO EXISTE CAMBIAR EL MENSAJE
	    	printf("Error Fatal: sin comando, se cierra la conexion\n");
	    	break;
	    }
        */
	    int valor = cl_validateInput(input,&parameters);
	    if(valor==1)
	    	callMethod(socket,&parameters);
	    else
	    	printf("Error Fatal: el comando es incorrecto use el comando HELP\n");
	}

    return 0;
}



