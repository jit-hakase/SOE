#ifndef _SOE_EPOLL_REACTOR_H
##ifdef _SOE_EPOLL_REACTOR_H

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct epoll_reactor
{
	int epoll_fd;
	int listen_fd;
	uint16_t size;
	struct epoll_event *events;
	int (*handle_accept)(int fd);
	void (*handle_recv)(int fd);
	void (*handle_close)(int fd);
	void (*handle_error)(int fd, int err);
};

struct epoll_reactor * epoll_reactor_create(int listen_fd, uint16_t size);
int epoll_reactor_loop(struct epoll_reactor* reactor);
void epoll_destroy(struct epoll_reactor *reactor)

#endif
