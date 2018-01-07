#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "CommandParser.h"
#include "Exception.h"
#include "Server.h"
#include "common.h"
#include "Event.h"

void removeTail(CommandParser * parser) {
	int i;
	for (i = parser->recvSize - 1; i > 0; i--) {
		if (parser->recv[i] != '\r' && parser->recv[i] != '\n') {
			break;
		}
		parser->recv[i] = '\0';
		parser->recvSize--;
	}
}

void commandParserRun(CommandParser * parser) {
	parser->event->trigger(parser->event, BeforeRun, (void *)parser);
	if (NULL == parser->recv) {
		strcpy(parser->exception->message, "receive message could not be null");
		parser->exception->ExcepType = 1;
		Throw(parser->exception);
	}

	removeTail(parser);
	char * token;
	token = strtok(parser->recv, DELIMITER);
	if (NULL == token) {
		strcpy(parser->exception->message, "invalid command");
		parser->exception->ExcepType = 1;
		Throw(parser->exception);
	}

	parser->command = token;
	int index = 0;
	while ((token = strtok(NULL, DELIMITER)) != NULL) {
		if (index >= INIT_PARAMS_SIZE) {
			strcpy(parser->exception->message, "invalid command");
			parser->exception->ExcepType = 1;
			Throw(parser->exception);
		}

		parser->params[index] = token;
		index++;
	}
	parser->paramsSize = index;
	parser->event->trigger(parser->event, AfterRun, (void *)parser);
}

void commandParserDestroy(void * object) {
	CommandParser * parser = (CommandParser *)object;
	parser->event->destroy(parser->event);
	free(parser->recv);

//	for(int i = 0; i < parser->paramsSize; i++) {
//		free(parser->params[i]);
//	}

	free(parser->waitingCheckCommand);
	free(parser->params);
	free(parser);
}

void commandParserSetRecv(CommandParser *parser, char * recv, int size) {
	if (NULL == recv) {
		strcpy(parser->exception->message, "recv could not be null");
		parser->exception->ExcepType = 1;
		Throw(parser->exception);
	}

	free(parser->recv);
	parser->recvSize = size;
	parser->recv = (char *)malloc(parser->recvSize * sizeof(char));
	memset(parser->recv, '\0', parser->recvSize);
	strcpy(parser->recv, recv);
}

void commandParserReflush(CommandParser * parser) {
	int i = 0;
	for (; i < parser->paramsSize; i++) {
		parser->params[i] = NULL;
	}

	parser->paramsSize = 0;
	
}

void * commandParserCheckCommand(void *object) {
	CommandParser * parser = (CommandParser *)object;
	if (strcmp(parser->waitingCheckCommand, parser->command) != 0) {
		strcpy(parser->exception->message, "check command falied");
		parser->exception->ExcepType = 1;
		Throw(parser->exception);
	}
}

CommandParser * initCommandParser(ExcepSign * exception) {
	CommandParser * parser = (CommandParser *)malloc(sizeof(CommandParser));
	bzero(parser, sizeof(CommandParser));
	parser->command = NULL;
	parser->recv = NULL;
	parser->recvSize = 0;
	parser->params = (char **)calloc(INIT_PARAMS_SIZE, sizeof(char *));
	parser->paramsSize = INIT_PARAMS_SIZE;
	parser->run = commandParserRun;
	parser->exception = exception;
	parser->event = initEvent();
	parser->reflush = commandParserReflush;
	parser->destroy = commandParserDestroy;
	parser->setRecv = commandParserSetRecv;
	parser->checkCommand = commandParserCheckCommand;
	return parser;
}
