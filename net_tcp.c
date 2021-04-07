int tcp_channel_init(int *sock)
{
	int ret;
	
	ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == ret) return -1;
	
	*sock = ret;
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
