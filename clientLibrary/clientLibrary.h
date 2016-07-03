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

int cl_get(int socket,dsString* key);
int cl_set(int socket,dsString* key,dsString* value);
int cl_list(int socket);
int cl_del(int socket,dsString* key);
//int callMethod(int socket,instruction* parameters);
//*******MODIFICAR disconnect*****///
int cl_disconnect(int socket);

void cl_help();
void cl_exit();
void sendKey(int sock,dsString* key);
void sendKeyValue(int sock,dsString* key,dsString* value);


#endif
