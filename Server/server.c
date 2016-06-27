#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#define workNumbers 2
#define MAX 5
//compilar gcc -o server server.c
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

void * worker(void* arg){
	int socket,read_size;
	char message[1000];
	while(1) {
		pthread_mutex_lock(&mutex);
		while (count == 0)
			pthread_cond_wait(&newConection_cv, &mutex); // se duerme el worker
		socket=getNewSocket(); //cuando se despierta guarda el socket que se acabo de conectar
		conectionTaking++;
		pthread_mutex_unlock(&mutex);

		while(1){// atiendo al cliente
			read_size = recv(socket , message , 1000 , 0);
			if(read_size>0){
				message[read_size]='\0';
			}else{
				continue;
			}
			send(socket , message , strlen(message),0);
			printf("sokect: %d, mensaje: %s\n", socket,message);
			pthread_mutex_lock(&mutex);
			conectionTaking--; // libero este worker
			if (conectionTaking < workNumbers && count>0){
				pthread_cond_signal(&newConection_cv);
			} 
			pthread_mutex_unlock(&mutex);
		}
	}

	return NULL;
}

/*
char *inputString(FILE* fp, size_t size){
 //The size is extended by the input with the value of the provisional 
     if(!fp){
     	return NULL;
     }
     char *str;
     int ch;
     size_t len = 0;
     str = realloc(NULL, sizeof(char)*size);//size is start size
     if(!str)return str;
 
     while(EOF!=(ch=fgetc(fp)) && ch != '\n'){  
         str[len++]=ch;
         if(len==size){
             str = realloc(str, sizeof(char)*(size+=16));
             if(!str)return str;
         }
     }
 	str[len++]='\0';
 	str = realloc(str, sizeof(char)*len);
 
     return str;
 }
*/

int main(int argc , char *argv[]){


	//BOSS
	int socket_desc ,c;
    struct sockaddr_in server , client;
	int i;
	//Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    c = sizeof(struct sockaddr_in);
    if (socket_desc == -1)
    {
        printf("no se pudo crear el socket...\n");
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 9999 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("ERROR, fallo el enlace\n");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);
    

	for(i=0; i< MAX; i++){
		newSockets[i]=0;
	}

	//create a thread pool
	for(i=0; i< workNumbers; i++){
		pthread_t p;
		pthread_create(&p, NULL, worker, NULL);
	}
	//
	puts("Esperando por clientes...");
	while (1) {
		int client_sock;
		
	    //accept connection from an incoming client
	    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	    if (client_sock < 0)
	    {
	        perror("No se acepto la conexion\n");
	        continue;
	    }

		pthread_mutex_lock(&mutex);
		if (conectionTaking == workNumbers){ //todos los workes estan ocupados
			printf("Ya no hay hilos para atenderlo, se perdio la conexion con el cliente: %d\n", client_sock);
			pthread_mutex_unlock(&mutex);
			continue;
		}
		putNewSocket(client_sock);
		pthread_cond_signal(&newConection_cv);
		pthread_mutex_unlock(&mutex);


	}



}