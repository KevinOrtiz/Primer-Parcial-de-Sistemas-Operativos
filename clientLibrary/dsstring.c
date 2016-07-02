#include "dsstring.h"
#include <stdlib.h>
#include <stdio.h>


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
    printf("\n");
}

void dsStringPrintChunk(dsString *s){
    dsChunk* it;
    for(it = s->header; it!=NULL ; it= it->next ){
        printf("%s", it->cont);
        printf("\n");
    }
    
}



