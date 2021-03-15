#include "cpu.h"

static void net_timeout_init(uint64_t *time_now)
{
	RDTSCP(*time_now);
}

static int net_timeout_check(const uint64_t time_start, const uint64_t time_gap)
{
	uint64_t time_dead, time_now;
	
	time_dead = time_start + time_gap;
	RDTSCP(time_now);
	
	return time_now >= time_dead;
}

static int net_nio_operate(int fd, char *buf, size_t len, ssize_t (*io_fn)(int, void*, size_t ,int))
{
	static uint64_t time_gap = 1000000000;
	uint64_t time_start;
	size_t remain;
	ssize_t io_ret_n;
	
	remain = len;
	net_timeout_init(&time_start);
	
	while (remain > 0) {
		io_ret_n = io_fn(fd, buf, remain, MSG_DONTWAIT);
		if (io_ret_n < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				if (net_timeout_check(time_start, time_gap)) {
					return -2;
				}
				io_ret_n = 0;
			} else {
				return -1;
			}
		} else if (io_ret_n == 0) {
			break;
		}
		remain -= io_ret_n;
		buf += io_ret_n;
	}
	return (int) (len - remain);
}

int net_nio_recvn(int fd, void *buf, size_t len)
{
	net_nio_operate(fd, (char*) buf, len, recv);
}

int net_nio_sendn(int fd, void *buf, size_t len)
{
	net_nio_operate(fd, (char*) buf, len, (ssize_t (*)(int, void*, size_t, int)) (send));
}

int net_set_nonblock(int fd)
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

int net_set_nolinger(int fd)
{
	struct linger lg;
	lg.l_onoff = 1;
	lg.l_linger = 0;	
	return setsockopt(fd, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
}

int net_set_nodelay(int fd)
{
	const int flag = 1;
	return setsockopt(fd, SOL_SOCKET, TCP_NODELAY, &flag, sizeof(flag));
}

ssize_t net_sendn(int fd, const char *buf, size_t len)
{
	size_t remain;
	ssize_t ret_n;
	
	remain = len;
	
	while (remain > 0) {
		ret_n = send(fd, buf, remain, MSG_DONTWAIT);
		if (ret_n < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				continue;
			} else {
				return -1;
			}
		} else if (ret_n == 0) {
			break;
		}
		remain -= ret_n;
		buf += ret_n;
	}
	return (ssize_t) (len - remain);
}
