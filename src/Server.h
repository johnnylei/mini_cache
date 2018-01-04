#ifndef __SERVER_H__
#define __SERVER_H__
#include "Event.h"
#include "Exception.h"
#include "HashTable.h"
typedef struct _server {
	char * recv;
	int recvSize;
	int fd;
	Event *event;
	ExcepSign * exception;
	HashTable * dataStorage;
	char * (* run)(struct _server *);
	int (* appendRecv)(struct _server *, char *, int);
	void (* destroy)(void *);
} Server;

Server * initServer();
#endif
