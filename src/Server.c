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
	CommandExecuter * executer = server->executer;
	CommandParser * parser = executer->parser;
	Try(server->exception) {
		server->event->trigger(server->event, BeforeRun, (void *)server);
		if (NULL == server->recv) {
			strcpy(server->exception->message, "recv could not be null");
			server->exception->ExcepType = 1;
			Throw(server->exception);
		}

		parser->setRecv(parser, server->recv, server->recvSize);
		parser->run(parser);
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
	server->exception->destroy(server->exception);
	server->event->destroy(server->event);
	server->executer->destroy(server->executer);
	free(server);
}

Server * initServer(int fd, HashTable * dataStorage) {
	Server * server = (Server *)malloc(sizeof(Server));
	bzero(server, sizeof(Server));
	server->recv = NULL;
	server->recvSize = 0;
	server->fd = fd;
	server->event = initEvent();
	server->exception = initException();
	server->dataStorage = dataStorage;
	server->executer = initCommandExecuter(dataStorage, server->exception);

	server->run = run;
	server->appendRecv = appendRecv;
	server->destroy = serverDestroy;

	return server;
}
