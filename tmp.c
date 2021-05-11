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


//tmp

#ifndef SONE_TCP_H
#define SONE_TCP_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int tcp_channel_t;

typedef struct {
	tcp_channel_t channel;
	uint16_t port;
} tcp_server_t;

static inline tcp_channel_t tcp_channel_init()
{
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

static inline int tcp_channel_bind(tcp_channel_t channel, uint16_t port)
{
	struct sockaddr_in svr_addr;
	
	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(port);
	
	return bind(channel, (struct sockaddr*) &svr_addr, sizeof(svr_addr));
}

static inline int tcp_channel_listen(tcp_channel_t channel)
{
	return listen(channel, SOMAXCONN);
}

static inline tcp_server_t * tcp_server_init(uint16_t port)
{
	tcp_server_t *svr;
	int ret;
	
	svr = malloc(sizeof(tcp_server_t));
	if (NULL == svr) return NULL;
	
	svr->channel = tcp_channel_init();
	if (svr->channel <= 0) return NULL;
	
	ret = tcp_channel_bind(svr->channel, port);
	if (ret < 0) return NULL;
	
	ret = tcp_channel_listen(svr->channel);
	if (ret < 0) return NULL;
	
	svr->port = port;
	return svr;
}

static inline void tcp_server_exit(tcp_server_t *svr)
{
	close(svr->channel);
	free(svr);
}

static inline tcp_channel_t tcp_server_accept(tcp_server_t *svr)
{
	return accept(svr->channel, NULL, NULL);
}

#endif




???
	class Buffer {
public:
	Buffer(size_t size) : m_size(size) {
		m_buf = calloc(1, size);
	}
	~Buffer() {
		free(m_buf);
	}
	
	bool is_ok() {
		if (read_size() >= 4) {
			uint32_t len = *(uint32_t*) (m_buf + m_ridx);
			if (len >= read_size()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	
	void get(char *buf) {
		uint32_t len = *(uint32_t*) (m_buf + m_ridx);
		memcpy(buf, m_buf+m_ridx, len+4);
		m_ridx += len+4;
	}
	
	int peek_len() {
		if (read_size() >= 4) {
			return (uint32_t*) (m_buf + m_ridx);
		}
		return -1;
	}
	
	bool append(char *buf, size_t size) {
		while (m_widx + size > m_size) {
			m_size = m_size * 2;
			m_buf = realloc(m_buf, m_size);
		}
		memcpy(m_buf + m_widx, buf, size);
		m_widx += size;
	}
	
	int read_size() {
		return m_widx - m_ridx;
	}
	
	void rewind() {
		m_ridx = m_widx = 0;
	}
private:
	char *m_buf;
	int m_ridx = 0;
	int m_widx = 0;
};

