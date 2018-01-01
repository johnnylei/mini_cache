#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Server.h"
#include "common.h"
#include "CommandParser.h"
#include "CommandExecuter.h"
#include "Exception.h"
#include "HashTable.h"

char * run(Server* server) {
	CommandExecuter * executer;
	Try(server->exception) {
		server->event->trigger(server->event, BeforeRun, (void *)server);
		CommandParser * parser = initCommandParser(server);
		parser->run(parser);

		executer = initCommandExecuter(server, parser);
		executer->run(executer);
		server->event->trigger(server->event, AfterRun, (void *)server);
	} CatchElse (server->exception) {
		return server->exception->message;
	}

	if (executer->result->flag != SUCCESS) {
		return executer->result->ret;
	}

	return "SUCCESS\n";
}

int appendRecv(Server * server, char *str, int size) {
	server->recvSize += size;
	strcat(server->recv, str);
	return 1;
}

Server * initServer(int fd, HashTable * dataStorage) {
	Server * server = (Server *)malloc(sizeof(Server));
	server->recv = (char *)malloc(sizeof(char));
	server->fd = fd;
	server->recvSize = 0;
	server->run = run;
	server->appendRecv = appendRecv;
	server->event = initEvent();
	server->exception = (ExcepSign *)malloc(sizeof(ExcepSign));
	server->dataStorage = dataStorage;
	return server;
}
