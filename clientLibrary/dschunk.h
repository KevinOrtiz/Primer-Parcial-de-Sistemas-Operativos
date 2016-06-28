
#ifndef _DSCHUNK_H
#define	_DSCHUNK_H

//data structure Chunk
typedef struct dsChunk{
    char* cont;
    struct dsChunk *next;
}dsChunk;

dsChunk *dschunkNew(char*  cont);
void dsChunkSetCont(dsChunk *c, char* cont);
char* dsChunkGetCont(dsChunk *c);
void dsChunkSetNext(dsChunk *c, dsChunk *d);
dsChunk * dsChunkGetNext(dsChunk *c);
void dsChunkDelete(dsChunk** c);


#endif	/* _DSCHUNK_H */

