#ifndef HEADER_FILE

#define MAX_COMMAND_LENGTH           4 //bytes
#define MAX_KEY_LENGTH       134217728//bytes
#define MAX_VALUE_LENGTH     2147483648//bytes

typedef enum {
	SUCCESS, //sin error 
	INSUFFICIENT_MEMORY, //falta de memoria al hacer malloc
	PARSE_ERROR, //error de parseo
	WRONG_ARGUMENT, //argumento incorrecto
	BIG_KEY, //cuando una clave es mas grande que el maximo permitido
	BIG_VALUE, //cuando un valor es mas grande que el maximo permitido
	LACK_OF_ARGUMENT //cuando a un comando le faltan argumentos
}errors;


#endif
