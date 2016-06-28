#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
/*
CREACION DEL SOCKET TCP SE DERIVA DE LOS SIGUIENYES PASOS
1.-CREAR  UN SOCKET CON LA LLAMADA AL SISTEMA SOCKET()
2.-SETEAR UNA DIRECCION Y UN NUMERO DE PUERTO A TRAVES DE LA FUNCION BIND()
3.-ESCUCHAR LAS conexiones CON EL METODO listen
4.-ACEPTAR UNA CONEXION CON LA LLAMADA AL SISTEMA ACCEPT().
5.-ENVIAR Y RECIBIR DATOS DEL SERVIDOR AL CLIENTE  USANDO LAS LLAMADAS
AL SISTEMA READ() Y WRITE()
*/
void procesandoInformacion(int socket);
int main( int argc, char *argv[] ) {

/*

Variables que son usadas para almancenar el estado del proceso
a ejecutarse en cada llamadas al sistema.

*/
   int sockfd;
   int newsockfd;
   int numberPuerto;
   int servidorCliente;
   int pid;
/*

Char buffer[255]:variable almacena los clientes que se conectan
a este servidor
*/
   char buffer[256];
/*
struct sockaddr_in : esta estructura guarda la imformacion con el tipo de ip que vamos a trabajar , tambien almacena el numero y la direccion Ip

*/
   struct sockaddr_in serv_addr, cli_addr;

/*



*/
   int  datos;

   /* First call to socket() function */

  /*

Nos devuelve un valor negativo si hubo un problema para efectuarse la conexiones caso contrario nos devuelve un
valor mayor a cero
  */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
////////////////////////////////////////////////
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   numberPuerto = 5000;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(numberPuerto);

   /*
   Esta funcion lo que hace es enlazar la llamada al sistema socket(), tambien
   mandamos la direccion del server, y la longitud de la direccion del server
   y validamos el valor obtenido, si el valor es menor a cero, hubo un problemas
   caso contrario la conexion fue exitosa.

   */
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }

   /*
  Ahora habilitamos el socket para que escuche en su puerto determinado
  el segundo argumento lo que hace es limitar el numero de conexiones que se encolan y esperan ser
  procesados
   */

   listen(sockfd,5);
   servidorCliente = sizeof(cli_addr);

   /* Aceptar conexiones que venga de diferentes clientes
   lo encerramos en un loop infinito para procesar, leer, escribir todas las peticiones que se encolan

    */
    while(1){
      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &servidorCliente);

      if (newsockfd < 0) {
         perror("ERROR on accept");
         exit(1);
      }

      /* If connection is established then start communicating */
      pid= fork();
      if(pid < 0){
        printf("No se pudo replicar el proceso \n");
        exit(1);
      }
      else if(pid == 0){

        close(sockfd);
        procesandoInformacion(newsockfd);
        exit(0);

      }
      else{
        close(newsockfd);
      }

    }
   return 0;
}
void procesandoInformacion(int socket){
  int informacion;
  char buffer[256];
  bzero(buffer,256);

  /*
  Leer datos de los clientes que se encuentran en el buffer
  pero primero coje en el que esta en el top de la pila
  */
  informacion = read(socket,buffer,255);


    if (informacion < 0){
      printf("Error leindo la informacion");
      exit(1);

    }
    printf("Here is the message: %s\n",buffer);


     informacion = write(socket,"I got your message",18);

     if (informacion < 0) {
        perror("ERROR writing to socket");
        exit(1);
     }


}
