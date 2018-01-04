#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "Server.h"
#include "common.h"
#include "CommandParser.h"
#include "CommandExecuter.h"
#include "Exception.h"
#include "HashTable.h"

char * run(Server* server) {
	CommandExecuter * executer;
	CommandParser * parser;
	Try(server->exception) {
		server->event->trigger(server->event, BeforeRun, (void *)server);
		parser = initCommandParser(server);
		parser->run(parser);

		executer = initCommandExecuter(server, parser);
		executer->run(executer);
		server->event->trigger(server->event, AfterRun, (void *)server);
	} CatchElse (server->exception) {
		parser->destroy(parser);
		executer->destroy(executer);
		return server->exception->message;
	}

	if (executer->result->flag != SUCCESS) {
//		parser->destroy(parser);
//		executer->destroy(executer);
		return executer->result->ret;
	}

	parser->destroy(parser);
	executer->destroy(executer);
	return "SUCCESS\n";
}

int appendRecv(Server * server, char *str, int size) {
	server->recvSize += size;
	int _size = server->recvSize * sizeof(char);
	char *tmp = (char *)malloc(_size);
	bzero(tmp, _size);
	if (NULL != server->recv) {
		strcpy(tmp, server->recv);
	}
	strcat(tmp, str);

	free(server->recv);
	server->recv = tmp;
	return 1;
}

void serverDestroy(void *object) {
	Server * server = (Server *)object;
	free(server->recv);
	free(server->exception);
	server->event->destroy(server->event);
	free(server);
}

Server * initServer(int fd, HashTable * dataStorage) {
	Server * server = (Server *)malloc(sizeof(Server));
	server->recv = NULL;
	server->fd = fd;
	server->recvSize = 0;
	server->run = run;
	server->appendRecv = appendRecv;
	server->destroy = serverDestroy;
	server->event = initEvent();
	server->exception = (ExcepSign *)malloc(sizeof(ExcepSign));
	server->dataStorage = dataStorage;

	return server;
}
