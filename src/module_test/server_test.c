#include <string.h>
#include <stdio.h>
#include "../Server.h"
#include "../HashTable.h"

int main(int argc, char ** argv) {
	HashTable * dataStorage = initHashWithSize(20);
	Server * server = initServer(1, dataStorage);
	char str[] = "set johnny nicolas";
	server->appendRecv(server, str, strlen(str));

	char * ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "get johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny xxx");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny aaaa");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lpush johnny bbbb");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "llen johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "lrange johnny 0 3");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "del johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);

	server = initServer(1, dataStorage);
	strcpy(str, "list johnny");
	server->appendRecv(server, str, strlen(str));
	ret = server->run(server);
	printf("%s\n", ret);
	server->destroy(server);
}
