#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define workNumbers 10

//compilar gcc -o server server.c
//ejecutar: ./server < entradas.txt > log.txt

//arrego de enteros
int sockets[workNumbers];
int fill = 0; //posicion donde se ingresan los datos
int use = 0; //posicion donde se leen los datos
int count = 0; //numero de datos


//variables de condicion
pthread_cond_t fill_cv = PTHREAD_COND_INITIALIZER;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_wait 
//pthread_cond_signal

void put(int value) {
	sockets[fill] = value;
	fill = (fill + 1) % workNumbers;
	count++;
}

int get() {
	int tmp = sockets[use];
	use = (use + 1) % workNumbers;
	count--;
	return tmp;
}

void * worker(void* arg){

	while(1) {
		pthread_mutex_lock(&mutex);
		while (count == 0)
			pthread_cond_wait(&fill_cv, &mutex); // se duerme el worker
		
		int tmp = get();
		pthread_mutex_unlock(&mutex);

		//hacer todas las operaciones
		//en este caso imprimir
		printf("Procese el sokect %d\n", tmp);
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
	//create a thread pool
	for(i=0; i< workNumbers; i++){
		pthread_t p;
		pthread_create(&p, NULL, worker, NULL);
	}
	//

	while (1) {

		//leer de stdin
		int socket; 
		
		char * input = inputString(stdin, 10);
		if(*input=='\0'){
			printf("Ya no hay mas entradas\n");
			return 0;
		}
		else{
			socket = atoi(input);
			printf("Lei %d\n", socket );
		}

		pthread_mutex_lock(&mutex);
		if (count == workNumbers){
			printf("Ya no hay hilos, perdio el dato %d\n", socket);
			pthread_mutex_unlock(&mutex);
			continue;
		}
		
		put(socket);
		pthread_cond_signal(&fill_cv);
		pthread_mutex_unlock(&mutex);


	}



}