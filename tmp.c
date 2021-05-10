inline int tcp_channel_init()
{
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

inline int udp_channel_init()
{
	return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

inline int tcp_channel_bind(int channel, uint16_t port)
{
	struct sockaddr_in svr_addr;
	
	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(port);
	
	return bind(channel, (struct sockaddr*) &svr_addr, sizeof(svr_addr));
}

inline int tcp_channel_listen(int channel)
{
	return listen(channel, SOMAXCONN);
}

enum {
	TC_NODELAY = TCP_NODELAY,
	TC_REUSEADDR = SO_REUSEADDR,
	TC_REUSEPORT = SO_REUSEPORT
}

inline int tcp_channel_set_default_option(channel)
{
	int ret;
	
	ret = tcp_channel_set_option(channel, TC_NODELAY);
	if (-1 == ret) return -1;
	
	ret = tcp_channel_set_option(channel, TC_REUSEADDR);
	if (-1 == ret) return -2;
	
	return 0;
}

inline int tcp_channel_set_option(int channel, int option)
{
	return setsockopt(channel, SOL_SOCKET, option, &on, sizeof(on));
}

inline int tcp_channel_connect(int channel)
{
	
}

