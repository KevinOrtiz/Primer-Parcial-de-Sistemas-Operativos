#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define workNumbers 1
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
	int socket;
	while(1) {
		pthread_mutex_lock(&mutex);
		while (count == 0)
			pthread_cond_wait(&newConection_cv, &mutex); // se duerme el worker
		socket=getNewSocket(); //cuando se despierta guarda el socket que se acabo de conectar
		conectionTaking++;
		pthread_mutex_unlock(&mutex);

		int contador=999999;
		while(contador!=0)
			contador--;
		printf("Procese el sokect: %d\n", socket);
		pthread_mutex_lock(&mutex);
		conectionTaking--; // libero este worker
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}


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


int main(){


	//BOSS
	int i;
	for(i=0; i< MAX; i++){
		newSockets[i]=0;
	}
	//create a thread pool
	for(i=0; i< workNumbers; i++){
		pthread_t p;
		pthread_create(&p, NULL, worker, NULL);
	}
	//

	while (1) {
		if(ban){
			//leer de stdin
			int socket; 
			
			char * input = inputString(stdin, 10);
			if(*input=='\0'){
				ban=0;
				continue;
			}
			else{
				socket = atoi(input);
				//printf("Servidor recibe %d\n", socket );
			}

			pthread_mutex_lock(&mutex);
			if (conectionTaking == workNumbers){ //todos los workes estan ocupados
				printf("Ya no hay hilos, perdio el dato %d\n", socket);
				pthread_mutex_unlock(&mutex);
				continue;
			}
			putNewSocket(socket);
			pthread_cond_signal(&newConection_cv);
			pthread_mutex_unlock(&mutex);
		}


	}



}