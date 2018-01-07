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
#include "User.h"

char * run(Server* server) { CommandExecuter * executer = server->executer;
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
		server->reflush(server);
		return server->exception->message;
	}

	server->reflush(server);
	return executer->result->ret;
}

int serverAppendRecv(Server * server, char *str, int size) {
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
	return SUCCESS;
}

void serverRefulsh(Server * server) {
	bzero(server->recv, server->recvSize);
	server->recvSize = 0;
	server->executer->reflush(server->executer);
}

void serverInitUserTable(Server * server) {
	HashTable * userTable;
	userTable = initHash();
	
	UserGroup * userGroup = initUserGroup("admin", STRLEN("admin"));
	UserData * userData = initUserData("root", STRLEN("root"), "root@minicache@123", STRLEN("root@minicache@123"), userGroup);
	Bucket * bucket = initBucket("root", (void *)userData, sizeof(UserData), DATA_TYPE_USER_DATA, userData->destroy);
	int ret = userTable->insert(userTable, bucket);
	if (ret != SUCCESS) {
		return ;
	}

	userGroup = initUserGroup("admin", STRLEN("admin"));
	userData = initUserData("admin", STRLEN("admin"), "admin@minicache@123", STRLEN("admin@minicache@123"), userGroup);
	bucket = initBucket("admin", (void *)userData, sizeof(UserData), DATA_TYPE_USER_DATA, userData->destroy);
	ret = userTable->insert(userTable, bucket);
	if (ret != SUCCESS) {
		return ;
	}
	server->userTable = userTable;
}

void * serverCheckUser(void * object) {
	Server * server = (Server *)object;
	char * fd = (char *)malloc(10);
	sprintf(fd, "%d", server->fd);
	Bucket * result;
	int ret = server->userClienMap->lookup(server->userClienMap, fd, &result);
	if(ret == SUCCESS) {
		return NULL;
	}

	if (strncmp(server->recv, "login", strlen("login")) == 0) {
		return NULL;
	}
	
	strcpy(server->exception->message, "access denied");
	Throw(server->exception);
}

void serverDestroy(void *object) {
	Server * server = (Server *)object;
	if (NULL != server->recv) {
		free(server->recv);
	}

	server->exception->destroy(server->exception);
	server->event->destroy(server->event);
	server->executer->destroy(server->executer);
	server->userTable->destroy(server->userTable);
	server->userClienMap->destroy(server->userClienMap);
	free(server);
}

Server * initServer(HashTable * dataStorage) {
	Server * server = (Server *)malloc(sizeof(Server));
	bzero(server, sizeof(Server));
	server->recv = NULL;
	server->recvSize = 0;
	server->fd = 0;
	server->event = initEvent();
	server->exception = initException();
	server->dataStorage = dataStorage;
	server->executer = initCommandExecuter(dataStorage, server->exception);
	serverInitUserTable(server);
	server->userClienMap = initHash();

	server->run = run;
	server->appendRecv = serverAppendRecv;
	server->reflush= serverRefulsh;
	server->destroy = serverDestroy;
	server->event->on(server->event, BeforeRun, serverCheckUser);

	return server;
}
