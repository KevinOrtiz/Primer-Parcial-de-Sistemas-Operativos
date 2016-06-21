
#ifndef _NODE_H
#define	_NODE_H

typedef struct NodeList{
    char* cont;
    struct NodeList *next;
}NodeList;

NodeList *nodeListNew(char*  cont);
void nodeListSetCont(NodeList *p, char* cont);
char* nodeListGetCont(NodeList *p);
void nodeListSetNext(NodeList *p, NodeList *q);
NodeList * nodeListGetNext(NodeList *p);


#endif	/* _NODE_H */

