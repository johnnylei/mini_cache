#ifndef __COMMAND_PARSER_H__
#define __COMMAND_PARSER_H__
#include "Exception.h"
#include "Server.h"
#include "Event.h"

#define INIT_PARAMS_SIZE 4
#define DELIMITER " "
typedef struct _commandParser {
	char * recv;
	int recvSize;
	char * command;
	char ** params;
	int paramsSize;
	void (* run)(struct _commandParser *);
	ExcepSign * exception;
	Event * event;
} CommandParser;

CommandParser * initCommandParser(Server *server);
#endif
