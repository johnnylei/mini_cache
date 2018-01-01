#ifndef __SERVER_H__
#define __SERVER_H__
#include "Event.h"
#include "Exception.h"
#include "HashTable.h"
typedef struct _server {
	char * recv;
	int recvSize;
	int fd;
	char * (* run)(struct _server *);
	int (* appendRecv)(struct _server *, char *, int);
	Event *event;
	ExcepSign * exception;
	HashTable * dataStorage;
} Server;

Server * initServer();
#endif
