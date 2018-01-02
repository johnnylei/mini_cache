#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CommandExecuter.h"
#include "CommandParser.h"
#include "Server.h"
#include "HashTable.h"
#include "common.h"
#include "Exception.h"
#include "Link.h"

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

int delValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The del command params len must be 1\n");
		Throw(executer->exception);
	}
	
	int ret = hash_remove(dataStorage, executer->parser->params[0]);
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

int listPushValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The lpush command params len must be 2\n");
		Throw(executer->exception);
	}

	int ret = hash_append(dataStorage, executer->parser->params[0], (void *)executer->parser->params[1]);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->flag = SUCCESS;
	return SUCCESS;
}

int listLen(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The llen command params len must be 1\n");
		Throw(executer->exception);
	}

	Link * values;
	int ret = hash_lookup(dataStorage, executer->parser->params[0], (void **)&values);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->ret = (char *)malloc(sizeof(char));
	sprintf(executer->result->ret, "%d", values->size);
	return SUCCESS;
}

int listRangeValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 3) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The lrange command params len must be 3\n");
		Throw(executer->exception);
	}

	int min = atoi(executer->parser->params[1]);
	int max = atoi(executer->parser->params[2]);
	if (min > max) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "lrange key min max; min should not bigger than max\n");
		Throw(executer->exception);
	}

	Link * values;
	int ret = hash_lookup(dataStorage, executer->parser->params[0], (void **)&values);
	if (ret == FAILED) {
		return FAILED;
	}

	
	executer->result->ret = (char *)malloc(sizeof(char));
	LinkNode * current = values->head;
	
	int index = 0;
	char * tmp;
	while(current) {
		if (index < min) {
			continue;
		}

		if (index > max) {
			break;
		}

		tmp = (char *)current->data;
		sprintf(tmp, "%s,", tmp);
		strcat(executer->result->ret, tmp);
		index++;
		current = current->next;
	}

	return SUCCESS;
}

int listValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The list command params len must be 1\n");
		Throw(executer->exception);
	}

	Link * values;
	int ret = hash_lookup(dataStorage, executer->parser->params[0], (void **)&values);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->ret = (char *)malloc(sizeof(char));
	LinkNode * current = values->head;
	
	char * tmp;
	while(current) {
		tmp = (char *)current->data;
		sprintf(tmp, "%s,", tmp);
		strcat(executer->result->ret, tmp);
		current = current->next;
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

	ret = hash_insert(commandHandlerMap, "del", (void *)delValue);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "lpush", (void *)listPushValue);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "llen", (void *)listLen);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "lrange", (void *)listRangeValue);
	if (ret == FAILED) {
		return FAILED;
	}

	ret = hash_insert(commandHandlerMap, "list", (void *)listValue);
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
