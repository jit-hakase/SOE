int tcp_channel_init(int *sock)
{
	int ret;
	
	ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == ret) return -1;
	
	*sock = ret;
	return 0;
}

int tcp_channel_listen(int sock, uint16_t port)
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

int tcp_channel_accept(int sock, int *clt_sock)
{
	int ret;
	
	ret = accept(sock, NULL, NULL);
	
	if (-1 == ret) return -1;
	
	*clt_sock = ret;
	return 0;
}

int tcp_channel_connect(int sock)
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
