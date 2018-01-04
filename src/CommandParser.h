#ifndef __COMMAND_PARSER_H__
#define __COMMAND_PARSER_H__
#include "Exception.h"
#include "Event.h"

#define INIT_PARAMS_SIZE 4
#define DELIMITER " "
typedef struct _commandParser {
	char * recv;
	int recvSize;
	char * command;
	char ** params;
	int paramsSize;
	ExcepSign * exception;
	Event * event;
	void (* run)(struct _commandParser *);
	void (* setRecv)(struct _commandParser *, char *, int);
	void (* destroy)(void *);
} CommandParser;

CommandParser * initCommandParser(ExcepSign *exception);
#endif
