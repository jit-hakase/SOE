void epoll_reactor_add(struct epoll_reactor* reactor, int fd)
{
	int ret;
	struct epoll_event event;
	
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLRDHUP;
	
	ret = epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, fd, &event);
	if (ret < 0) {
		reactor->handle_error(fd, -2);
	}
}

void epoll_reactor_del(struct epoll_reactor* reactor, int fd)
{
	int ret;
	
	ret = epoll_ctl(reactor->epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	if (ret < 0) {
		reactor->handle_error(fd, -3);
	}
}

int epoll_reactor_loop(struct epoll_reactor* reactor)
{
	int rdy_num, idx, ret, fd, clt_fd;
	uint32_t evt_flag;
	
	rdy_num = epoll_wait(reactor->epoll_fd, reactor->events, reactor->size, -1);
	if (-1 == rdy_num) return -1;
	
	for (idx = 0; idx < rdy_num; ++idx) {
		fd = reactor->events[idx].data.fd;
		evt_flag = reactor->events[idx].events;
		if (evt_flag & (EPOLLERR | EPOLLRDHUP)) {
			epoll_reactor_del(reactor, fd);
			if (evt_flag & EPOLLERR) {
				reactor->handle_error(fd, -1);	
			}
			reactor->handle_close(fd);
		} else if (evt_flag & EPOLLIN) {
			if (fd == reactor->listen_fd) {
				clt_fd = reactor->handle_accept(fd);
				if (clt_fd > 0) {
					epoll_reactor_add(reactor, fd);
				}
			} else {
				reactor->handle_recv(fd);
			}
		}
	}
	return rdy_num;
}

struct epoll_reactor * epoll_reactor_create(int listen_fd, uint16_t size)
{
	struct epoll_reactor *reactor;
	struct epoll_event svr_evt;
	int ret;
	
	reactor = malloc(sizeof(struct epoll_reactor));
	reactor->size = size;
	reactor->events = calloc(size + 1, sizeof(struct epoll_event));
	reactor->epoll_fd = epoll_create1(0);
	if (reactor->epoll_fd < 0) return NULL;
	
	svr_evt.data.fd = listen_fd;
	svr_evt.events = EPOLLIN;
	ret = epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, listen_fd, &svr_evt);
	if (ret < 0) return NULL;
	
	return reactor;	
}

void epoll_destroy(struct epoll_reactor *reactor)
{
	free(reactor->events);
	free(reactor);
}
