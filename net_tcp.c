inline int tcp_channel_init(int *sock)
{
	int ret;
	
	ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == ret) return -1;
	
	*sock = ret;
	return 0;
}

inline int tcp_channel_listen(int sock, uint16_t port)
{
	struct sockaddr_in svr_addr;
	const int on = 1;
	int ret;
	
	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (-1 == ret) return -1;
	
	bzero(&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(port);
	
	ret = bind(sock, (struct sockaddr*) &svr_addr, sizeof(svr_addr));
	if (-1 == ret) return -2;
	
	ret = listen(sock, SOMAXCONN);
	if (-1 == ret) return -3;
	
	return 0;
}

inline int tcp_channel_accept(int sock, int *clt_sock)
{
	int ret;
	
	ret = accept(sock, NULL, NULL);
	
	if (-1 == ret) return -1;
	
	*clt_sock = ret;
	return 0;
}

inline int tcp_channel_connect(int sock)
{
	struct sockaddr_in svr_addr;
	int ret;
	
	bzero(&svr_addr, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr = inet_aton(ip_addr);
	svr_addr.sin_port = port;
	
	ret = connect(sock, (struct sockaddr*) &svr_addr, sizeof(svr_addr))
	if (-1 == ret) return -1;
	
	return 0;
}

inline ssize_t tcp_channel_send(int fd, const void *buf, size_t len)
{
	return send(fd, buf, len, MSG_WAITALL);
}

inline ssize_t tcp_channel_recv(int fd, void *buf, size_t len)
{
	return recv(fd, buf, len, MSG_WAITALL);
}

inline ssize_t udp_channel_send(int fd, const void *buf, size_t len)
{
	return send(fd, buf, len, 0);
}

inline ssize_t udp_channel_recv(int fd, void *buf, size_t max_len)
{
	return recv(fd, buf, max_len, 0);
}

inline int tcp_set_nolinger(int fd)
{
	struct linger lg;
	lg.l_onoff = 1;
	lg.l_linger = 0;	
	return setsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
}

inline int tcp_set_nodelay(int fd)
{
	const int flag = 1;
	return setsockopt(fd, SOL_SOCKET, TCP_NODELAY, &flag, sizeof(flag));
}

inline int net_set_nonblock(int fd)
{
	int flag;
	
	flag = fcntl(fd, F_GETFL, 0);
	if (-1 == flag) return -1;
	
	flag |= O_NONBLOCK;
	if (-1 == flag) return -1;
	
	flag = fcntl(fd, F_SETFL, flag);
	if (-1 == flag) return -1;
	
	return 0;
}
