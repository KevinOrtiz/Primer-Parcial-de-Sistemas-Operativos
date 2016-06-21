#include "list.h"
#include <stdlib.h>


List *listNew(){
    List *newList = malloc(sizeof(List));
    newList->header = newList->last = NULL;
    return newList;
}
int listIsEmpty(List *L){
    return (L->header == NULL && L->last == NULL);
}
NodeList *listGetHeader(List *L){ return L->header; }

NodeList *listGetLast(List *L) { return L->last; }

void listAddNode(List *L, NodeList *newNode){
    nodeListSetNext(newNode, NULL);
    if(listIsEmpty(L))
        L->header = L->last = newNode;
    else {
        nodeListSetNext(L->last, newNode);
        L->last = newNode;
    }
}

void listAddFirst(List *L, NodeList *newNode){
    if(listIsEmpty(L)){
        L->header = L->last = newNode;
    }else{
        newNode->next = L->header;
        L->header = newNode;
    }
}


NodeList *listRemoveFirst(List *L){
    NodeList *tmp;
    if(listIsEmpty(L)) return NULL;
    tmp = L->header;
    if(L->header == L->last) L->last = NULL;
    L->header = L->header->next;
    tmp->next = NULL;
    return tmp;
}


void listDelete(List **L){

    free(*L);
}



