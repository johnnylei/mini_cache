#ifndef __COMMAND_ExECUTER_H__
#define __COMMAND_ExECUTER_H__
#include "HashTable.h"
#include "Exception.h"
#include "CommandParser.h"
#include "Server.h"
#include "Event.h"
typedef union _result {
	int flag;
	char * ret;
} Result;

typedef struct _commandExecuter {
	ExcepSign * exception;
	HashTable * dataStorage;
	HashTable * commandHandlerMap;
	CommandParser * parser;
	Event * event;
	Result * result;
	void (* run)(struct _commandExecuter *);
} CommandExecuter;

CommandExecuter * initCommandExecuter(Server *server, CommandParser * parser);
#endif
