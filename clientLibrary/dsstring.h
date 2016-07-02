#ifndef _DSSTRING_H
#define	_DSSTRING_H

#include "dschunk.h"


#define CHUNK_LENGTH 20
//data structure String
typedef struct dsString{
    dsChunk *header, *last;
    int countChar;
}dsString;


dsString* dsStringNew();
int dsStringEmpty(dsString *s);
char* dsStringGetHeader(dsString *s);
char* dsStringGetLast(dsString *s);
void dsStringAdd(dsString *s, char* cont);
void dsStringAddFirst(dsString *s, char* cont);                                                                                                                                                                                                                                                                                                                                                                       

int dsStringAddChar(dsString* s, char c);
char* dsStringRemoveFirst(dsString *s);

void dsStringDelete(dsString **s);

void dsStringPrint(dsString *s);
void dsStringPrintChunk(dsString *s);


#endif	/* _DSSTRING_H */

