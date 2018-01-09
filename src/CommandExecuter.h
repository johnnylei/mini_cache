#ifndef __COMMAND_ExECUTER_H__
#define __COMMAND_ExECUTER_H__
#include "HashTable.h"
#include "Exception.h"
#include "CommandParser.h"
#include "Event.h"
#include "Link.h"

#define SUBSCRIBE_MAP_SIZE 1024
typedef struct _commandExecuterresult {
	char * ret;
	unsigned long int retSize;
	void (* destroy)(void *);
	void (* reflush)(struct _commandExecuterresult *);
	void (* setRet)(struct _commandExecuterresult *, const char *, unsigned long);
} CommandExecuterResult;

typedef struct _commandExecuter {
	int fd;
	ExcepSign * exception;
	HashTable * dataStorage;
	HashTable * commandHandlerMap;
	HashTable * userTable;
	HashTable * userClientMap;
	// key:queue value:clientfd
	HashTable * subscribeClientMap;
	// key:clientfd value:queue
	HashTable * clientSubscribeMap;
	CommandParser * parser;
	Event * event;
	CommandExecuterResult * result;
	void (* run)(struct _commandExecuter *);
	void (* destroy)(void *);
	void (* reflush)(struct _commandExecuter *);
	void (* clientClose)(struct _commandExecuter *);
} CommandExecuter;

CommandExecuter * initCommandExecuter(HashTable * dataStorage, HashTable * userTable, HashTable * userClientMap, ExcepSign * exception);
CommandExecuterResult * initCommandExecuterResult();
#endif
