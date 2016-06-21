#ifndef _LIST_H
#define	_LIST_H

#include "nodelist.h"


typedef struct List{
    NodeList *header, *last;
}List;


List *listNew();
int listIsEmpty(List *L);
NodeList *listGetHeader(List *L);
NodeList *listGetLast(List *L);
void listAddNode(List *L, NodeList *newNode);
void listAddFirst(List *L, NodeList *newNode);                                                                                                                                                                                                                                                                                                                                                                       
NodeList *listRemoveFirst(List *L);
void listDelete(List **L);


#endif	/* _LIST_H */

