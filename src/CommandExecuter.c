#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "CommandExecuter.h"
#include "CommandParser.h"
#include "Server.h"
#include "HashTable.h"
#include "common.h"
#include "Exception.h"
#include "Link.h"

#define ItemHashTableInitSize 128
#define RET_SUCCESS "SUCCESS"

HashTable * commandHandlerMap;
void commandExecuterRun(CommandExecuter * executer) {
	executer->event->trigger(executer->event, BeforeRun, (void *)executer);
	Bucket * result;
	int ret = commandHandlerMap->lookup(commandHandlerMap, executer->parser->command, &result);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		sprintf(executer->exception->message, "unknow command %s\n", executer->parser->command);
		Throw(executer->exception);
	}
	
	
	int (* handler)(CommandExecuter *) = (int (*)(CommandExecuter *))result->value;
	ret = handler(executer);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		sprintf(executer->exception->message, "command %s execute failed\n", executer->parser->command);
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

	Bucket * bucket = initBucket(executer->parser->params[0], (void *)executer->parser->params[1], strlen(executer->parser->params[1]) + 1, DATA_TYPE_STRING, free);
	int ret = dataStorage->insert(dataStorage, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

int delValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The del command params len must be 1\n");
		Throw(executer->exception);
	}
	
	int ret = dataStorage->remove(dataStorage, executer->parser->params[0]);
	if (ret == FAILED) {
		return FAILED;
	}
	
	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

int getValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The get command params len must be 1\n");
		Throw(executer->exception);
	}

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		sprintf(executer->exception->message, "could not found key=%s\n", executer->parser->params[0]);
		Throw(executer->exception);
	}

	if (result->valueType != DATA_TYPE_STRING) {
		return FAILED;
	}

	
	executer->result->setRet(executer->result, (char *)result->value, result->valueSize);
	return SUCCESS;
}

int listPushValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The lpush command params len must be 2\n");
		Throw(executer->exception);
	}

	LinkNode * node = initLinkNode((void *)executer->parser->params[1], strlen(executer->parser->params[1]) + 1, DATA_TYPE_STRING, free);
	Link * link;
	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		link = initLink();
		link->append(link, node);
		Bucket * bucket = initBucket(executer->parser->params[0], link, 0, DATA_TYPE_LINK, link->destroy);
		dataStorage->insert(dataStorage, bucket);
		executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
		return SUCCESS;
	}

	if (result->valueType != DATA_TYPE_LINK) {
		return FAILED;
	}

	link = (Link *)result->value;
	link->append(link, node);

	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

int listLen(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The llen command params len must be 1\n");
		Throw(executer->exception);
	}

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		return FAILED;
	}

	if (result->valueType != DATA_TYPE_LINK) {
		return FAILED;
	}

	Link * values = (Link *)result->value;
	int size = 10 * sizeof(char);
	if (size > executer->result->retSize) {
		executer->result->retSize = size;
		free(executer->result->ret);
		executer->result->ret = (char *)malloc(executer->result->retSize);
	}

	bzero(executer->result->ret, executer->result->retSize);
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

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		return FAILED;
	}

	if (result->valueType != DATA_TYPE_LINK) {
		return FAILED;
	}
	
	Link * values = (Link *)result->value;
	LinkNode * current = values->head;

	int index = 0, _index = 0;
	unsigned long size = 0;
	char ** tmp = (char **)calloc((max - min + 1),  sizeof(char *));
	while (current) {
		if (index < min) {
			index++;
			continue;
		}

		if (index > max) {
			break;
		}

		tmp[_index] = (char *)current->value;
		size += current->valueSize;
		current = current->next;
		index++;
		_index++;
	}

	// 数据长度+n个','的长度
	size = size + _index * sizeof(char);
	if (size > executer->result->retSize) {
		free(executer->result->ret);
		executer->result->retSize = size;
		executer->result->ret = (char *)malloc(size);
	}
	memset(executer->result->ret, '\0', executer->result->retSize);

	int i;
	for (i = 0; i < _index; i++) {
		strcat(executer->result->ret, tmp[i]);
		strcat(executer->result->ret, ",");
	}
	free(tmp);
	
	return SUCCESS;
}

int listValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 1) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The list command params len must be 1\n");
		Throw(executer->exception);
	}

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		return FAILED;
	}

	if (result->valueType != DATA_TYPE_LINK) {
		return FAILED;
	}

	Link * values = (Link *)result->value;
	unsigned long size = 0;
	LinkNode * current = values->head;
	char ** tmp = (char **)calloc(values->size, sizeof(char));
	int index = 0;
	while (current) {
		tmp[index] = (char *)current->value;
		size += current->valueSize;
		current = current->next;
		index++;
	}

	size += values->size * sizeof(char);
	if (size > executer->result->retSize) {
		free(executer->result->ret);
		executer->result->retSize = size;
		executer->result->ret = (char *)malloc(size);
	}
	bzero(executer->result->ret, executer->result->retSize);
	int i;
	for (i = 0; i < values->size; i++) {
		strcat(executer->result->ret, tmp[i]);
		strcat(executer->result->ret, ",");
	}
	free(tmp);

	return SUCCESS;
}

int listDelValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The ldel command params len must be 2\n");
		Throw(executer->exception);
	}

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED) {
		return FAILED;
	}

	if (result->valueType != DATA_TYPE_LINK) {
		return FAILED;
	}

	Link * values = (Link *)result->value;
	int offset = atoi(executer->parser->params[1]);
	ret = values->del(values, offset);
	if (ret == FAILED) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "del item failed, please check the index is exists\n");
		Throw(executer->exception);
	}

	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

// hashmap
int hashmapSetValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 3) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The hmset command params len must be 3\n");
		Throw(executer->exception);
	}

	HashTable * hashTable;
	Bucket * bucket = initBucket(executer->parser->params[1], (void *)executer->parser->params[2], strlen(executer->parser->params[2]) + 1, DATA_TYPE_STRING, free);
	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == SUCCESS) {
		if (result->valueType != DATA_TYPE_HASHTABLE) {
			return FAILED;
		}

		hashTable = (HashTable *)result->value;
		hashTable->insert(hashTable, bucket);
		executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
		return SUCCESS;
	}

	hashTable = initHashWithSize(ItemHashTableInitSize);
	hashTable->insert(hashTable, bucket);

	bucket = initBucket(executer->parser->params[0], (void *)hashTable, 0, DATA_TYPE_HASHTABLE, hashTable->destroy);
	ret = dataStorage->insert(dataStorage, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

int hashmapGetValue(CommandExecuter *executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The hmget command params len must be 2\n");
		Throw(executer->exception);
	}
	
	Bucket *result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED || result->valueType != DATA_TYPE_HASHTABLE) {
		return FAILED;
	}


	HashTable * hashTable = (HashTable *)result->value;
	ret = hashTable->lookup(hashTable, executer->parser->params[1], &result);
	if (ret == FAILED) {
		return FAILED;
	}

	executer->result->setRet(executer->result, (char *)result->value, result->valueSize);
	return SUCCESS;
}

int hashmapDelValue(CommandExecuter * executer) {
	HashTable * dataStorage = executer->dataStorage;
	if (executer->parser->paramsSize != 2) {
		executer->exception->ExcepType = 2;
		strcpy(executer->exception->message, "The hmdel command params len must be 2\n");
		Throw(executer->exception);
	}

	Bucket * result;
	int ret = dataStorage->lookup(dataStorage, executer->parser->params[0], &result);
	if (ret == FAILED || result->valueType != DATA_TYPE_HASHTABLE) {
		return FAILED;
	}

	HashTable * hashTable = (HashTable *)result->value;
	ret = hashTable->remove(hashTable, executer->parser->params[1]);
	if (ret == FAILED) {
		return FAILED;
	}

	
	executer->result->setRet(executer->result, RET_SUCCESS, STRLEN(RET_SUCCESS));
	return SUCCESS;
}

int initCommandHandlerMap() {
	if (NULL != commandHandlerMap) {
		return SUCCESS;
	}

	commandHandlerMap = initHashWithSize(20);
	if (commandHandlerMap == NULL) {
		return FAILED;
	}

	Bucket * bucket = initBucket("set", (void *)setValue, 0, DATA_TYPE_CALLBACK, NULL);
	int ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("get", (void *)getValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("del", (void *)delValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("lpush", (void *)listPushValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("llen", (void *)listLen, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("lrange", (void *)listRangeValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("list", (void *)listValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("ldel", (void *)listDelValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("hmset", (void *)hashmapSetValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("hmget", (void *)hashmapGetValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	bucket = initBucket("hmdel", (void *)hashmapDelValue, 0, DATA_TYPE_CALLBACK, NULL);
	ret = commandHandlerMap->insert(commandHandlerMap, bucket);
	if (ret == FAILED) {
		return FAILED;
	}

	return SUCCESS;
}

void commandExecuterResultReflush(CommandExecuterResult * result) {
//	bzero(result->ret, result->retSize);
}

void commandExecuterResultDestroy(void * object) {
	CommandExecuterResult * result = (CommandExecuterResult *)object;
	free(result->ret);
	free(result);
}

void commandExecuterResultSetRet(CommandExecuterResult * result, const char * str, unsigned long size) {
	if (result->retSize < size) {
		result->retSize = size;
		free(result->ret);
		result->ret = (char *)malloc(size);
	}
	bzero(result->ret, result->retSize);
	strcpy(result->ret, str);
}

CommandExecuterResult * initCommandExecuterResult() {
	CommandExecuterResult * result = (CommandExecuterResult *)malloc(sizeof(CommandExecuterResult));
	bzero(result, sizeof(CommandExecuterResult));
	result->ret = NULL;
	result->retSize = 0;

	result->reflush = commandExecuterResultReflush;
	result->setRet = commandExecuterResultSetRet;
	result->destroy = commandExecuterResultDestroy;
	return result;
}

void commandExecuterReflush(CommandExecuter * executer) {
	executer->parser->reflush(executer->parser);
	executer->result->reflush(executer->result);
}

void commandExecuterDestroy(void * object) {
	CommandExecuter * executer = (CommandExecuter *)object;
	executer->event->destroy(executer->event);
	executer->result->destroy(executer->result);
	executer->parser->destroy(executer->parser);
	free(executer);
}

CommandExecuter * initCommandExecuter(HashTable * dataStorage, ExcepSign * exception) {
	if (initCommandHandlerMap() == FAILED) {
		exception->ExcepType = 2;
		strcpy(exception->message, "init command hander map failed");
		Throw(exception);
	}

	CommandExecuter * executer = (CommandExecuter *)malloc(sizeof(CommandExecuter));
	bzero(executer, sizeof(CommandExecuter));
	executer->parser = initCommandParser(exception);
	executer->exception = exception;
	executer->run = commandExecuterRun;
	executer->event = initEvent();
	executer->result = initCommandExecuterResult();
	executer->dataStorage = dataStorage;
	executer->reflush = commandExecuterReflush;
	executer->destroy = commandExecuterDestroy;
	return executer;
}
