#ifndef HEADER_FILE
#define HEADER_FILE
#define MAXKEY 128000

typedef struct instruction {
	char command[5];
	char key[MAXKEY];
	FILE* value;
}instruction;


int cl_validateInput(instruction* parameters);

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
