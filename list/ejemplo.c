#include "nodelist.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

// gcc -o prog ejemplo.c nodelist.c list.c

int main(){
	List* L = listNew();

	int i = 0;
	for (i=0; i< 10; i++){
		NodeList* n = nodeListNew("Mensaje");
		listAddNode(L,n);
	}

	NodeList * it;
	for(it = L->header; it!=NULL; it = it->next){
		printf("%s\n", it->cont);
	}
	return 0;
}