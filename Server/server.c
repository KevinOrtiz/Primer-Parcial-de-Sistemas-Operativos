#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write, close
#include "constants.h"
#include "bstrlib.h"
#include "Hash.h"
#include "../datastructures/dsstring.h"

#define workNumbers 2
#define MAX 5

//compilar gcc -o server server.c Hash.c bstrlib.c darray.c ../datastructures/dsstring.c ../datastructures/dschunk.c
//ejecutar: ./server

int conectionTaking = 0; //numero de conexiones atendidas
int newSocket=0;
//arrego de enteros
int newSockets[MAX];
int actual=0;
int first=0;
int count=0;
int ban=1;

//hashMap
Hashmap *map;

//variables de condicion
pthread_cond_t newConection_cv = PTHREAD_COND_INITIALIZER;
//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static uint32_t Hashmap_djb2_hash(void *data); //funcion de hash
int commandNumArguments(char* command);
void putNewSocket(int s);
int getNewSocket();
void* worker(void* arg);
int initServerSocket(int* socket_desc, int port);
int reciveAllChunks(int socket,dsString *s);
int exec(int socket,char * command, dsString* key, dsString* value);
DArray* getResultList();

int main(int argc , char *argv[]){


	if(argc<=1){
		printf("Error fatal: especifique un puerto\n");
		return 0;
	}
	int port = atoi(argv[1]);
	if(!port){
		printf("Error fatal: puerto incorrecto\n");
		return 0;
	}
	//create a hash
	map = Hashmap_create(dsStringCmp, Hashmap_djb2_hash);

	//BOSS
	int socket_desc,val,i;
    val=initServerSocket(&socket_desc, port);
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
			//enviarle ese mensaje al cliente
			send(client_sock, "CONECTION_NK", strlen("CONECTION_NK"),0);
			close(client_sock);

			continue;
		}
		if(count==MAX){
			printf("ERROR(count):El servidor esta a su maxima capacidad, se perdio la conexion con el cliente: %d\n", client_sock);
			pthread_mutex_unlock(&mutex);

			//enviarle ese mensaje al cliente
			send(client_sock, "CONECTION_NK", strlen("CONECTION_NK"),0);
			close(client_sock);

			continue;
		}
		//confirma coneccion
		send(client_sock, "CONECTION_OK", strlen("CONECTION_OK"),0);

		putNewSocket(client_sock);
		pthread_cond_signal(&newConection_cv);
		pthread_mutex_unlock(&mutex);


	}


}

DArray* getResultList(){
	DArray *array,*bucket;
	DArray *response;
	array=map->buckets;
	HashmapNode *n;
	response=DArray_create(sizeof(dsString*),DEFAULT_EXPAND_RATE);
	printf("*****RESULTADOS DEL LIST*****\n");
	for (int i = 0; i < DEFAULT_NUMBER_OF_BUCKETS; ++i){
		//printf("i:%d\n",i );
		bucket=DArray_get(array,i);
		if(!bucket)
			continue;
		for (int j = 0; j < bucket->end; ++j){
			//printf("j:%d\n",j );
			n=DArray_get(bucket,j);
			if(!n)
				break;
			//printf("\ncubeta: %d, pos: %d, Key: ",i,j);
			DArray_push(response,n->key);
		}
	}
	return response;
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
	int x = newSockets[first];
	first=(first+1)%MAX;
	count--;
	return x;
}
int reciveAllChunks(int socket,dsString *s){
	int read_size;
	char *chunk;
	while(1){//recibe todo los chunk de de clave o valor
		chunk=(char*)malloc(sizeof(char)*(CHUNK_LENGTH+1));
		read_size = recv(socket , chunk , CHUNK_LENGTH+1, 0);
		if(read_size>0) 
			chunk[read_size]='\0';
		else{
			send(socket , "ERROR" , strlen("ERROR"),0);
			return -1;
		}
		send(socket , "OK" , strlen("OK"),0);
		if(!strcmp(chunk,"<<<fin_cadena>>>"))
			break;
		//printf("%s",chunk );
		dsStringAdd(s,chunk);
		fflush(stdout);
	}
	dsStringPrint(s);
	return 1;
}

int exec(int socket,char * command, dsString* key, dsString* value){
	DArray *list;
	if(!command)
		return WRONG_ARGUMENT;
	if(strcmp(command,"GET")==0){
        printf("Se ejecuta get\n");

        value = (dsString *)Hashmap_get(map, key);
        if(value){
        	printf("Bien\n");
        	dsStringPrint(value);
        	return -1;
        }
        
        return 1; //necesita un solo paramentro
    }
    if(strcmp(command,"SET")==0){
    	printf("Se ejecuta set\n");
    	int result = Hashmap_set(map, key, value);
    
    	if(result<0){
    		return INSUFFICIENT_MEMORY;
    	}

        return SUCCESS; //necesitan dos paramentros
    }
    if(strcmp(command,"LIST")==0){
    	list=getResultList();
    	dsString *k;
    	for (int i = 0; i < list->end; i++){
			//printf("j:%d\n",j );
			k=DArray_get(list,i);
			printf("key: ");
			dsStringPrint(k);

		}
    	printf("Se ejecuta list\n");
        return 0; //no se necesitan paramentros
    }
    if(strcmp(command,"DEL")==0){
    	printf("se ejecuta del\n");
        return 1; //necesita un solo paramentro
    }
	return WRONG_ARGUMENT;
}


void * worker(void* arg){
	int socket,read_size, i,ban;
	char command[10];
	dsString *key,*value;
	while(1) {
		pthread_mutex_lock(&mutex);
		while (count == 0)
			pthread_cond_wait(&newConection_cv, &mutex); // se duerme el worker
		socket=getNewSocket(); //cuando se despierta guarda el socket que se acabo de conectar
		conectionTaking++;
		pthread_mutex_unlock(&mutex);

		while(1){// atiendo al cliente
			key=dsStringNew();
			value=dsStringNew();
			read_size = recv(socket , command , 10, 0);
			if(read_size>0){
				command[read_size]='\0';	
			} 
			else{
				send(socket , "ERROR" , strlen("ERROR"),0);
				close(socket);
				break;	
			} 
			send(socket , "OK" , strlen("OK"),0);
			printf("\nsokect: %d, comando: %s\n", socket,command);
			if(!strcmp(command,"EXIT")){
				close(socket);
				break;
			}
			int num=commandNumArguments(command);
			ban=1;
			for(i=0;i<num;i++){
				if(i==0){
					printf("Key:");
					ban=reciveAllChunks(socket,key);
					if(ban!=1)
						break;
				}
				if(i==1){
					printf("Value:");
					ban=reciveAllChunks(socket,value);
					if(ban!=1)
						break;
				}
			}
			if(ban!=1){
				printf("este es el problema\n");
				break;
			}
			exec(socket,command,key,value);
			printf("PILAS\n");

		}
		

		pthread_mutex_lock(&mutex);
		conectionTaking--; // libero este worker
		pthread_mutex_unlock(&mutex);
		printf("Se Desconecto sokect: %d\n", socket);
	}

	return NULL;
}

int initServerSocket(int* socket_desc, int port){
	struct sockaddr_in server , client;
	int i;
	//Create socket
    *socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (*socket_desc == -1)
        return 0;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    //Bind
    if( bind(*socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
        return 0;
    //Listen
    listen(*socket_desc , 3);
    return 1;
}



static uint32_t Hashmap_djb2_hash(void *data)
{

    dsString* s = (dsString*)data;
    unsigned long hash = 5381;

    dsChunk* it;

    for(it=s->header; it !=NULL; it=it->next){
        char * str = it->cont;

        while (*str!= '\0'){
            hash = ((hash << 5) + hash) + *str;
            str++;
        }     

    }

    return hash;
}
