#ifndef HEADER_FILE
#define HEADER_FILE


int cl_validateInput(char* input, char** parameters, int numParams);

//********MODIFICAR CONNECT*******
char* cl_connect();

char* cl_get(char* key);
char* cl_set(char* key, char* value);
char* cl_list();
char* cl_del(char* key);

//*******MODIFICAR disconnect*****///
char* cl_disconnect();

#endif