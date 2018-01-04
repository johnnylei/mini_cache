#ifndef __COMMAND_ExECUTER_H__
#define __COMMAND_ExECUTER_H__
#include "HashTable.h"
#include "Exception.h"
#include "CommandParser.h"
#include "Server.h"
#include "Event.h"
typedef struct _commandExecuterresult {
	int flag;
	char * ret;
	void (* destroy)(void *);
} CommandExecuterResult;

typedef struct _commandExecuter {
	ExcepSign * exception;
	HashTable * dataStorage;
	HashTable * commandHandlerMap;
	CommandParser * parser;
	Event * event;
	CommandExecuterResult * result;
	void (* run)(struct _commandExecuter *);
	void (* destroy)(void *);
} CommandExecuter;

CommandExecuter * initCommandExecuter(Server *server, CommandParser * parser);
CommandExecuterResult * initCommandExecuterResult();
#endif
