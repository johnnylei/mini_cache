#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Server.h"
#include "../HashTable.h"

int main(int argc, char ** argv) {
	HashTable * dataStorage = initHashWithSize(20);
	char *str = (char *)malloc(65536);

	Server * server = initServer(dataStorage);
	server->fd = 1;
	strcpy(str, "login admin admin@sminicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	char * ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 2;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 3;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 4;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 5;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 6;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 1;
	strcpy(str, "subscribe johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 2;
	strcpy(str, "subscribe johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 3;
	strcpy(str, "subscribe johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 4;
	strcpy(str, "subscribe johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 5;
	strcpy(str, "subscribe johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 6;
	strcpy(str, "publish johnny xxxx");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	server->fd = 1;
	server->clientClose(server);
	server->fd = 2;
	server->clientClose(server);
	server->fd = 3;
	server->clientClose(server);
	server->fd = 4;
	server->clientClose(server);
	server->fd = 5;
	server->clientClose(server);

	server->fd = 1;
	strcpy(str, "login admin admin@minicache@123");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "set johnny xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssssxxxsssssssssssssssssssss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "lpush johnny xxx");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "lpush johnny aaaa");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "lpush johnny bbbb");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "llen johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "ldel johnny 1");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "lrange johnny 0 3");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmset johnny nicolas xxx");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmset johnny wesley ssss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmset johnny lili iiiiss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmget johnny lili");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmget johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmget johnny nicolassss");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmdel johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "hmget johnny nicolas");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);

	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str) + 1);
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);
}
