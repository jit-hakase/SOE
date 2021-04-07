#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <libnet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>

int bind_interface(int sock, const char *device)
{
	struct sockaddr_ll sll;
	struct ifreq ifr;
	int ret;
	
	bzero(&sll, sizeof(sll));
	bzero(&ifr, sizeof(ifr));
	
	strncpy(ifr.ifr_name, device, IFNAMSIZ);
	
	ret = ioctl(sock, SIOCGIFINDEX, &ifr);
	if (-1 == ret) return -1;
	
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);
	
	ret = bind(sock, (struct sockaddr*) &sll, sizeof(sll));
	
	if (-1 == ret) return -2;
	
	return 0;
}

int set_nic_promisc(int sock, const char *device)
{
	struct ifreq ifr;
	int ret;
	
	strncpy((char*) ifr.ifr_name, device, IFNAMSIZ);
	
	ret = ioctl(sock, SIOCGIFFLAGS, &ifr);
	if (-1 == ret) return -1;
	ifr.ifr_flags |= IFF_PROMISC;
	ret = ioctl(sock, SIOCSIFFLAGS, &ifr);
	if (-1 == ret) return -2;
	return 0;
}

int main()
{
	int sock;
	ssize_t n;
	char buf[2048];
	
	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	
	bind_interface(sock, argv[1]);
	
	set_nic_promisc(sock, argv[1]);
	
	for (;;) {
		n = recv(sock, buf, sizeof(buf), 0);
		printf("%d\n", n);
	}
	return 0;
}
