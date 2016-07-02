#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char *argv[]) {
   int sockfd;
   int datos;
   int numeroPuerto;
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[256];
/*
argc son los argumentos que se pueden ingresar por teclado al momento de ejecutar el programa

*/
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }
/*
atoi: Permite capturar el valor ingresado por teclado al momento de ejecutar el programa cliente

*/
   numeroPuerto = atoi(argv[2]);

   /*

Creacion de socket y guardo el valor entero del estado de creacion en esta Variable
sockfd,

   */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }

  /*
  esta funcion procesa la direccion ip del valor ingresado por teclado  y este valor es capturado por agrv
  y lo guardamos en una variable que apunta a la estructura host

  */


   server = gethostbyname(argv[1]);
/*

si el serviddor es nulo entonces da entender que no se ingresado ninguna direccion IP

*/
   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(numeroPuerto);

      /*
      Ahora conectamos con el servidor indicando el puerto por donde esta escuchando el server*/
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
           perror("ERROR connecting");
           exit(1);
        }

     /* Now ask for a message from the user, this message
        * will be read by server
     */


            printf("Please enter the message: ");
            //bzero(buffer,256);
            fgets(buffer,255,stdin);

            /*

            write, envimos los datos al servidor con sta funcion

             */
            datos = write(sockfd, buffer, strlen(buffer));

            if (datos < 0) {
               perror("ERROR writing to socket");
               exit(1);
            }

            /* Now read server response */
            bzero(buffer,256);
            /*
            Ahora leiemos la respuesta del servidor
            */
            datos = read(sockfd, buffer, 255);

            if (datos < 0) {
               perror("ERROR reading from socket");
               exit(1);
            }

            printf("%s\n",buffer);       
   return 0;
}
