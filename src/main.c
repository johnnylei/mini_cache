#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include "Server.h"
#include "HashTable.h"

#define set_nonblocking(fd) do { \
	int option = fcntl(fd, F_GETFL); \
	fcntl(fd, F_SETFL, option | O_NONBLOCK); \
} while(0);

#define MAX_EVENT_NUMBER 1024
#define BUFF_SIZE 1024
#define DATA_STORAGE_INIT_SIZE 65536

typedef struct accept_client_param {
	int server_fd;
	int epfd;
} accept_client_param_t;

void * accept_client(void * context) {
	accept_client_param_t * param = (accept_client_param_t *)context;
	int client_fd;

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd_len;
	while (client_fd = accept(param->server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) {
		struct epoll_event event;
		event.data.fd = client_fd;
		event.events = EPOLLIN | EPOLLET;
		epoll_ctl(param->epfd, EPOLL_CTL_ADD, client_fd, &event);
		set_nonblocking(client_fd);
	}
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("server [ip]\n");
		return 0;
	}

	HashTable * dataStorage = initHashWithSize(DATA_STORAGE_INIT_SIZE);
	Server * server = initServer(dataStorage);
	struct sockaddr_in server_address;
	server_address.sin_port = ntohs(12345);
	server_address.sin_family = AF_INET;
	inet_aton(argv[1], &server_address.sin_addr);

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(server_fd >= 0);

	int ret = bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));
	assert(ret >= 0);

	ret = listen(server_fd, 5);
	assert(ret >= 0);

	int epfd = epoll_create(5);
	assert(epfd >= 0);

	pthread_t handle;
	accept_client_param_t param;
	param.server_fd = server_fd;
	param.epfd = epfd;

	ret = pthread_create(&handle, NULL, accept_client, &param);
	assert(ret == 0);

	struct epoll_event events[MAX_EVENT_NUMBER];
	char read_buff[BUFF_SIZE];
	char * send_buff;

	int is_closed = 1;
	while(1) {
		ret = epoll_wait(epfd, events, MAX_EVENT_NUMBER, -1);
		assert(ret >= 0);

		int i;
		for (i = 0; i < ret; i++) {
			int client_fd = events[i].data.fd;
			server->fd = client_fd;

			if (events[i].events & EPOLLIN) {
				while (1) {
					memset(read_buff, '\0', BUFF_SIZE);
					ret = recv(client_fd, read_buff, BUFF_SIZE - 1, 0);
					if (ret < 0) {
						if (errno == EAGAIN || errno == EWOULDBLOCK) {
							break;
						}

						// 连接断开
						server->clientClose(server);
						close(client_fd);
						break;
					}

					if (ret == 0) {
						// 连接断开
						server->clientClose(server);
						close(client_fd);
						break;
					}
					
					is_closed = 0;
					server->appendRecv(server, read_buff, ret);
				}

				if (is_closed) {
					break;
				}

				send_buff = server->run(server);
				send(client_fd, send_buff, strlen(send_buff) + 1, 0);
			}

			if (events[i].events & EPOLLRDHUP || events[i].events & EPOLLHUP) {
				// 连接断开
				server->clientClose(server);
				close(client_fd);
			}
		}
	}

	server->destroy(server);
	close(server_fd);
}
