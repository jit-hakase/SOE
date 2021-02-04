static int nio_n(int fd, char *buf, size_t len, ssize_t (*io_fn)(int, void*, size_t ,int))
{
	size_t remain;
	ssize_t io_ret_n;
	
	remain = len;
	
	while (remain > 0) {
		io_ret_n = io_fn(fd, buf, remain, MSG_DONTWAIT);
		if (io_ret_n < 0) {
			if (errno == EAGAIN || errno == EINTR) {
				io_n = 0;
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

int nio_recv_n(int fd, void *buf, size_t len)
{
	nio_n(fd, (char*) buf, len, recv);
}

int nio_send_n(int fd, void *buf, size_t len)
{
	nio_n(fd, (char*) buf, len, (ssize_t (*)(int, void*, size_t, int)) (send));
}
