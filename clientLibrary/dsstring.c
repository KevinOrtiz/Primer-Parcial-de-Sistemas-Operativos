#include "dsstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <string.h>


dsString *dsStringNew(){
    dsString* newString = malloc(sizeof(dsString));
    newString->header = NULL;
    newString->last = NULL;
    newString->countChar = 0;
    return newString;
}

int dsStringEmpty(dsString *s){
    return (s->header == NULL && s->last == NULL);
}

char* dsStringGetHeader(dsString *s){
    dsChunk* chunk = s->header;
    return dsChunkGetCont(chunk);
}

char* dsStringGetLast(dsString *s){
    dsChunk* chunk = s->last;
    return dsChunkGetCont(chunk);
}

void dsStringAdd(dsString *s, char* cont){
    dsChunk* chunk = dschunkNew(cont);

    if(dsStringEmpty(s)){
        s->header = s->last = chunk;
    }
    else{
        dsChunkSetNext(s->last,chunk);
        s->last = chunk;
    }
}
void dsStringAddFirst(dsString *s, char* cont){
    dsChunk* chunk = dschunkNew(cont);
    if(dsStringEmpty(s)){
        s->header = s->last = chunk;
    }
    else{
        chunk->next = s->header;
        s->header = chunk;
    }
}

int dsStringAddChar(dsString* s, char c){

    if(s->countChar == 0){
        char * cont = (char*)malloc(sizeof(char)*(CHUNK_LENGTH + 1));
        if(!cont){
            return -1;
        }
        cont[0] = c;
        s->countChar = 1;
        dsStringAdd(s, cont);
        return 0;
    }

    if(s->countChar == CHUNK_LENGTH - 1 ){
        char * cont = s->last->cont;
        cont[CHUNK_LENGTH - 1] = c;
        cont[CHUNK_LENGTH] = '\0';
        s->countChar = 0;
        return 0;
    }

    if(s->countChar < CHUNK_LENGTH){
        char * cont = s->last->cont;
        cont[s->countChar] = c;
        (s->countChar)++;
    }

    return 0;
}

char* dsStringRemoveFirst(dsString *s){
    dsChunk* tmp;
    if(dsStringEmpty(s)) return NULL;
    tmp = s->header;
    if(s->header == s->last) s->last = NULL;
    s->header = s->header->next;
    char* cont = tmp->cont;
    free(tmp);
    return cont;
}

void dsStringDelete(dsString **s){
    while((*s)->header!=NULL){
        char * cont = dsStringRemoveFirst(*s);
        free(cont);
    }
    free(*s);
}

void dsStringPrint(dsString *s){
    dsChunk* it;
    for(it = s->header; it!=NULL ; it= it->next ){
        printf("%s", it->cont);
    }
}

void dsStringPrintChunk(dsString *s){
    dsChunk* it;
    for(it = s->header; it!=NULL ; it= it->next ){
        printf("%s", it->cont);
        printf("\n");
    }

}

int dsStringSendChunkSocket(dsString *s,int sock){
    dsChunk* it;
    char server_reply[10];
    int read_size;
    for(it = s->header; it!=NULL ; it= it->next ){
        char* aux;
        aux=it->cont;
        if(!strcmp(aux,""))
            break;
        if( send(sock , aux , strlen(aux) , 0) < 0){
            return -1;
        }
        read_size = recv(sock , server_reply , 2 , 0);
        server_reply[read_size]='\0';
        if(strcmp(server_reply,"OK")){
            return -1;
        }
    }
    if( send(sock , "<<<fin_cadena>>>" , strlen("<<<fin_cadena>>>") , 0) < 0)
        return -1;
    read_size = recv(sock , server_reply ,2 , 0);
    server_reply[read_size]='\0';
    if(strcmp(server_reply,"OK")){
        return -1;
    }
    return 0;

}

int dsStringCmp(void *a, void *b)
{
    //*a es la clave que esta en el hash y *b es la clave que voy a comparar
    dsString *hashKey, *newKey;
    hashKey = (dsString *)a;
    newKey = (dsString *)b;
    dsChunk* iterador1, *iterador2;
    int resultado;
    

    for( iterador1 = hashKey->header , iterador2 = newKey->header ; iterador1 != NULL && iterador2 != NULL ; iterador1 = iterador1->next , iterador2 = iterador2->next  ){
        resultado = strcmp(iterador1->cont,iterador2->cont);
        if(resultado != 0) return 1;//no son iguales
    }
    return 0;//son iguales
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
            send(socket , "NK" , strlen("NK"),0);
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

int reciveAllChunksPrint(int socket){
    int read_size;
    char chunk[CHUNK_LENGTH+1];
    int i=0;
    while(1){//recibe todo los chunk de de clave o valor
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
        printf("%s",chunk);
        fflush(stdout);
        i++;
    }
    if(i==0){
      printf("\n");
      return 1; 
    }
    return 1;
}

int reciveAllKeysPrint(int socket){
    int read_size;
    char chunk[CHUNK_LENGTH+1];
    int i=0;
    //printf("RECIBE ALL KEYS PRINT");
    printf("\t\tKEYS\n");
    while(1){
        while(1){//recibe todo los chunk de de clave o valor
            i++;
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
            if(!strcmp(chunk,"<<<fin_keys>>>"))
                break;
            printf("%s",chunk);
            fflush(stdout);
        }
        printf("\n");
        if(!strcmp(chunk,"<<<fin_keys>>>"))
            break;
    }
    return 1;
}



