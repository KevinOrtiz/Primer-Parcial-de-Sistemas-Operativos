#ifndef HEADER_FILE
#define HEADER_FILE
#define MAXKEY 128000
#define MAX 6
#include "../list/list.h"

typedef struct instruction {
	char command[5];
	char key[MAXKEY];
	List* value;
}instruction;

void printHelp();
List* inputString(FILE* fp);
int cl_validateInput(List* input,instruction* parameters);

//********MODIFICAR CONNECT*******
int cl_connect(char *argv[]);

char* cl_get(char* key);
char* cl_set(char* key, FILE* value);
char* cl_list();
char* cl_del(char* key);
int callMethod(int socket,instruction* parameters);
//*******MODIFICAR disconnect*****///
void cl_disconnect(int socket);


#endif
