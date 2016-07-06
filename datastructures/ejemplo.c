#include "dschunk.h"
#include "dsstring.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// gcc -o prog ejemplo.c nodelist.c list.c
int inputString(FILE* fp, dsString* s){
 //The size is extended by the input with the value of the provisional 
     if(!fp){
     	return -1;
     }
    int ch;
 	int band = 0;

     while(EOF!=(ch=fgetc(fp)) && ch != '\n'){  
     	 band = 1;
         dsStringAddChar(s, ch);
     }
     return band;
 }

int main(){
	dsString* s = dsStringNew();

	while(inputString(stdin, s)){
			dsChunk* it;
			for(it = s->header; it!=NULL ; it= it->next ){
				printf("__%s__", it->cont);
			} 	
			printf("\n");
			dsStringDelete(&s);
			s = dsStringNew();	
	}
	printf("fin\n");



	return 0;
}