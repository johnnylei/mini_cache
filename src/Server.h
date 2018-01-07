#ifndef __SERVER_H__
#define __SERVER_H__
#include "Event.h"
#include "User.h"
#include "Exception.h"
#include "HashTable.h"
#include "CommandExecuter.h"

typedef struct _server {
	char * recv;
	int recvSize;
	int fd;
	Event *event;
	ExcepSign * exception;
	HashTable * dataStorage;
	HashTable * userTable;
	HashTable * userClienMap;
	CommandExecuter * executer;

	char * (* run)(struct _server *);
	int (* appendRecv)(struct _server *, char *, int);
	void (* reflush)(struct _server *);
	void (* destroy)(void *);
} Server;

Server * initServer(HashTable * dataStorage);
#endif
