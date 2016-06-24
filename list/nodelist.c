#include "nodelist.h"
#include <stdlib.h>
#include <stdio.h>

NodeList *nodeListNew(char* cont){
    NodeList *newNode = malloc(sizeof(NodeList));
    newNode->next = NULL;
    newNode->cont = cont;
    return newNode;
}
void nodeListSetCont(NodeList *p, char* cont){
    p->cont = cont;
}
char* nodeListGetCont(NodeList *p){
    return p->cont;
}
void nodeListSetNext(NodeList *p, NodeList *q){
    p->next = q;
}
NodeList *nodeListGetNext(NodeList *p){
    return p->next;
}

void nodeListDelete(NodeList** p){
	free((*p)->cont);
	free(*p);
}
