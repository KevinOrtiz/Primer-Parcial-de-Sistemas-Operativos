#include "dschunk.h"
#include <stdlib.h>
#include <stdio.h>


dsChunk *dschunkNew(char*  cont){
    dsChunk* newChunk = malloc(sizeof(dsChunk));
    newChunk->next = NULL;
    newChunk->cont = cont;
    return newChunk;
}

void dsChunkSetCont(dsChunk *c, char* cont){
    c->cont = cont;
}
char* dsChunkGetCont(dsChunk *c){
    return c->cont;
}
void dsChunkSetNext(dsChunk *c, dsChunk *d){
    c->next = d;
}
dsChunk * dsChunkGetNext(dsChunk *c){
    return c->next;
}

void dsChunkDelete(dsChunk** c){
    free(*c);
}
