#ifndef HEADER_FILE
#include "dsstring.h"
#include "constants.h"
/*typedef struct instruction {
	char command[5];
	char key[MAXKEY];
	List* value;
}instruction;*/



void cl_printHelp();
int cl_inputString(FILE* fp, char* command, dsString* key, dsString* value);
int cl_validateCommand(char* command);
void cl_printError(int errorCode);
int cl_exec(char* command, dsString* key, dsString* value);
//********MODIFICAR CONNECT*******
int cl_connect(char *argv[]);

char* cl_get(char* key);
char* cl_set(char* key, FILE* value);
char* cl_list();
char* cl_del(char* key);
//int callMethod(int socket,instruction* parameters);
//*******MODIFICAR disconnect*****///
void cl_disconnect(int socket);


#endif
