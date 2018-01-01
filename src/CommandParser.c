#include <stdlib.h>
#include <string.h>
#include "CommandParser.h"
#include "Exception.h"
#include "Server.h"
#include "common.h"
#include "Event.h"

void removeTail(CommandParser * parser) {
	for (int i = parser->recvSize - 1; i > 0; i--) {
		if (parser->recv[i] != '\r' && parser->recv[i] != '\n') {
			break;
		}
		parser->recv[i] = '\0';
		parser->recvSize--;
	}
}

void commandParserRun(CommandParser * parser) {
	parser->event->trigger(parser->event, BeforeRun, (void *)parser);
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

CommandParser * initCommandParser(Server * server) {
	CommandParser * parser = (CommandParser *)malloc(sizeof(CommandParser));
	parser->recv = server->recv;
	parser->recvSize = server->recvSize;
	parser->command = NULL;
	parser->params = (char **)calloc(INIT_PARAMS_SIZE, sizeof(char *));
	parser->paramsSize = INIT_PARAMS_SIZE;
	parser->run = commandParserRun;
	parser->exception = server->exception;
	parser->event = initEvent();
	return parser;
}
