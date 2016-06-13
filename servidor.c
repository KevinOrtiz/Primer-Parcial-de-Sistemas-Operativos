#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <inttypes.h>

#if 0
/*
* Structs exported from in.h
*/

/* Internet address */
struct in_addr {
	unsigned int s_addr;
};

/* Internet style socket address */
struct sockaddr_in  {
	unsigned short int sin_family; /* Address family */
	unsigned short int sin_port;   /* Port number */
	struct in_addr sin_addr;	 /* IP address */
	unsigned char sin_zero[...];   /* Pad to size of 'struct sockaddr' */
};

#endif

int abrir_socket();
int enlazar_al_puerto( struct sockaddr_in *addr_server, int socket, int puerto);

int main(int argc, char *argv[]) {
	struct sockaddr_in server;
	struct sockaddr_in cliente;
	int _socket, puerto = -1, _bind, respuesta, optval;
	socklen_t size;

	puerto = atoi(argv[1]);
	printf("puerto:%d\n", puerto);
	_socket = abrir_socket();
	if( _socket < 0 ){
		printf("Error al abrir el socket\n");
		return;
	}
	optval = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR,
			   (const void *)&optval , sizeof(int));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htons(INADDR_ANY);
	server.sin_port = htons(puerto);

	if (bind(_socket, (struct sockaddr*)&server, sizeof(server)) < 0){
		printf("Error binding socket...");
		exit(1);
	}

	if( listen(_socket , 3) == -1 ){
		printf("Error en el listen\n");
		return;
	}

	printf("Enlazado al puerto\n");
	for(;;){
		size = sizeof(struct sockaddr_in);
		respuesta = accept(_socket , (struct sockaddr *) &cliente , &size);
		if(respuesta < 0){
			printf("Error en accept\n");
			return;
		}
		printf("Se obtuvo una conexion desde %s\n", inet_ntoa(cliente.sin_addr) );
		send(respuesta,"Bienvenido a mi servidor.\n",25,0);
		/* mostrara el mensaje de bienvenida al cliente */

		close(respuesta);
	}

	return 0;
}

int abrir_socket(){
	int s = socket(AF_INET, SOCK_STREAM, 0);
	return s;
}


