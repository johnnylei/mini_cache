#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Server.h"
#include "../HashTable.h"

int main(int argc, char ** argv) {
	HashTable * dataStorage = initHashWithSize(20);
	char *str = (char *)malloc(128);

	Server * server = initServer(1, dataStorage);
	strcpy(str, "set johnny xxx");
	server->appendRecv(server, str, strlen(str) + 1);
	char * ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny xxx");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny aaaa");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny bbbb");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "llen johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "ldel johnny 1");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lrange johnny 0 3");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmset johnny nicolas xxx");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmset johnny wesley ssss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmset johnny lili iiiiss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmget johnny lili");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmget johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmget johnny nicolassss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmdel johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "hmget johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);
}
