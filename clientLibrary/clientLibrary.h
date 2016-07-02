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
int cl_exec(int sock,char* command, dsString* key, dsString* value);
//********MODIFICAR CONNECT*******
int cl_connect(char* ip, char* puerto);

char* cl_get(char* key);
char* cl_set(char* key, FILE* value);
<<<<<<< HEAD
char* cl_list(int socket);
=======
char* cl_list();
>>>>>>> 72e25317bd101b15b0939bb656f1b9c8c894f1cc
char* cl_del(char* key,int socket);
//int callMethod(int socket,instruction* parameters);
//*******MODIFICAR disconnect*****///
void cl_disconnect(int socket);

void cl_help();

#endif
