#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CommandExecuter.h"
#include "CommandParser.h"
#include "Server.h"
#include "HashTable.h"
#include "common.h"
#include "Exception.h"

HashTable * commandHandlerMap;
void commandExecuterRun(CommandExecuter * executer) {
	executer->event->trigger(executer->event, BeforeRun, (void *)executer);
	int (* handler)(CommandExecuter *);
	int ret = hash_lookup(commandHandlerMap, executer->parser->command, (void **)&handler);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		char * message = (char *)malloc(sizeof(char));
		sprintf(message, "unknow command %s\n", executer->parser->command);
		strcpy(executer->exception->message, message);
		Throw(executer->exception);
	}
	
	ret = handler(executer);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		char * message = (char *)malloc(sizeof(char));
		sprintf(message, "command %s execute failed\n", executer->parser->command);
		strcpy(executer->exception->message, message);
		Throw(executer->exception);
	}

	executer->event->trigger(executer->event, AfterRun, (void *)executer);
}

int setValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The set command params len must be 2\n");
		Throw(executer->exception);
	}

	int ret = hash_insert(dataStorage, executer->parser->params[0], (void *)executer->parser->params[1]);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->flag = SUCCESS;
	return SUCCESS;
}

int getValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The get command params len must be 1\n");
		Throw(executer->exception);
	}

	int ret = hash_lookup(dataStorage, executer->parser->params[0], (void **)&executer->result->ret);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		char * message = (char *)malloc(sizeof(char));
		sprintf(message, "could not found key=%s\n", executer->parser->params[0]);
		strcpy(executer->exception->message, message);
		Throw(executer->exception);
	}

	return SUCCESS;
}

int initCommandHandlerMap() {
	if (NULL != commandHandlerMap) {
		return SUCCESS;
	}

	commandHandlerMap = (HashTable *)malloc(sizeof(HashTable));
	if (commandHandlerMap == NULL) {
		return FAILED;
	}

	int ret = hash_init(commandHandlerMap);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "set", (void *)setValue);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "get", (void *)getValue);
	if (ret == FAILED) {
		return FAILED;
	}

	return SUCCESS;
}

CommandExecuter * initCommandExecuter(Server * server, CommandParser * parser) {
	if (initCommandHandlerMap() == FAILED) {
		server->exception->ExcepType = 2;
		strcpy(server->exception->message, "init command hander map failed");
		Throw(server->exception);
	}

	CommandExecuter * executer = (CommandExecuter *)malloc(sizeof(CommandExecuter));
	executer->parser = parser;
	executer->exception = server->exception;
	executer->run = commandExecuterRun;
	executer->event = initEvent();
	executer->result = (Result *)malloc(sizeof(Result));
	executer->dataStorage = server->dataStorage;
	return executer;
}
