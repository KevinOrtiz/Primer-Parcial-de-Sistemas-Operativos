#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include "../datastructures/dsstring.h"

#define workNumbers 2
#define MAX 5
//compilar gcc -o server server.c -pthread
//ejecutar: ./server < entradas.txt > log.txt

//arrego de enteros
int conectionTaking = 0; //numero de conexiones atendidas
int newSocket=0;
int newSockets[MAX];
int actual=0;
int first=0;
int count=0;
int ban=1;

//variables de condicion
pthread_cond_t newConection_cv = PTHREAD_COND_INITIALIZER;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_wait
//pthread_cond_signal
int commandNumArguments(char* command);
void putNewSocket(int s);
int getNewSocket();
void* worker(void* arg);
int initServerSocket(int* socket_desc, char *argv[]);
void reciveAllChunks(int socket);

int main(int argc , char *argv[]){

	//BOSS
	int socket_desc,val,i;
    val=initServerSocket(&socket_desc, argv);
    if(!val){
    	printf("Error: Faltal no se puede iniciar el Servidor\n");
    	return 0;
    }
	for(i=0; i< MAX; i++){
		newSockets[i]=0;
	}
	//create a thread pool
	for(i=0; i< workNumbers; i++){
		pthread_t p;
		pthread_create(&p, NULL, worker, NULL);
	}
	printf("Servidor iniciado correctamente, esperando Clientes...\n");
	while (1) {
		int client_sock,c;
		struct sockaddr_in client;
		c = sizeof(struct sockaddr_in);
	    //accept connection from an incoming client
	    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	    if (client_sock < 0)
	    {
	        printf("ERROR: No se acepto la conexion\n");
	        continue;
	    }

		pthread_mutex_lock(&mutex);
		if (conectionTaking == workNumbers){ //todos los workes estan ocupados
			printf("ERROR:El servidor esta a su maxima capacidad, se perdio la conexion con el cliente: %d\n", client_sock);
			pthread_mutex_unlock(&mutex);
			continue;
		}
		if(count==MAX){
			printf("ERROR(count):El servidor esta a su maxima capacidad, se perdio la conexion con el cliente: %d\n", client_sock);
			pthread_mutex_unlock(&mutex);
			continue;
		}
		putNewSocket(client_sock);
		pthread_cond_signal(&newConection_cv);
		pthread_mutex_unlock(&mutex);


	}


}

int commandNumArguments(char* command){
    if(strcmp(command,"GET")==0){
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"SET")==0){
        return 2; //necesitan dos paramentros
    }
    if(strcmp(command,"LIST")==0){
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command,"DEL")==0){
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"EXIT")==0){
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command, "HELP")==0){
        return 0; //no se necesitan paramentros
    }
    return -1; //error comando no reconocido

}

void putNewSocket(int s){
	newSockets[actual]=s;
	actual=(actual+1)%MAX;
	count++;
}

int getNewSocket(){
	int x=newSockets[first];
	first=(first+1)%MAX;
	count--;
	return x;
}
void reciveAllChunks(int socket){
	int read_size;
	char chunk[1000];
	while(1){//recibe todo los chunk de de clave o valor
		read_size = recv(socket , chunk , 1000 , 0);
		if(read_size>0) chunk[read_size]='\0';
		send(socket , "OK" , strlen("OK"),0);
		if(!strcmp(chunk,"<<<fin_cadena>>>"))
			break;
		printf("%s",chunk );
		fflush(stdout);
	}
	printf("\n");
}

void * worker(void* arg){
	int socket,read_size, i;
	char command[10];
	while(1) {
		pthread_mutex_lock(&mutex);
		while (count == 0)
			pthread_cond_wait(&newConection_cv, &mutex); // se duerme el worker
		socket=getNewSocket(); //cuando se despierta guarda el socket que se acabo de conectar
		conectionTaking++;
		pthread_mutex_unlock(&mutex);

		while(1){// atiendo al cliente
			read_size = recv(socket , command , 1000 , 0);
			if(read_size>0) command[read_size]='\0';
			else continue;
			send(socket , "OK" , strlen("OK"),0);
			printf("\nsokect: %d, comando: %s\n", socket,command);
			if(!strcmp(command,"EXIT")){
				break;
			}
			int num=commandNumArguments(command);
			for(i=0;i<num;i++){
				if(i==0)
					printf("Key:");
				if(i==1)
					printf("Value:");
				reciveAllChunks(socket);
			}

		}
		pthread_mutex_lock(&mutex);
		conectionTaking--; // libero este worker
		pthread_mutex_unlock(&mutex);
		printf("Se Desconecto sokect: %d\n", socket);
	}

	return NULL;
}

int initServerSocket(int* socket_desc, char *argv[]){
	struct sockaddr_in server , client;
	int i;
	//Create socket
    *socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (*socket_desc == -1)
        return 0;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 9999 );
    //Bind
    if( bind(*socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        return 0;
    //Listen
    listen(*socket_desc , 3);
    return 1;
}
