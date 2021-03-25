#include <stdint.h>
#include <pcap.h>

void pcap_sniff(uint8_t *arg, const struct pcap_pkthdr *pkt_hdr, const uint8_t *pkt)
{
	struct pcap *dvc;
	
	dvc = (struct pcap*) (arg);
	
	/* ... */
	
	pcap_breakloop(dvc);
}

/* filter example = "ip src 192.168.2.1 and ip dst 192.168.2.2 || vlan" */

int pcap_sniff_setup(const char *nic_name, const char *pkt_filter)
{
	pcap_t *dvc;
	char err_buf[1024];
	struct bpf_program fcode;
	int ret;
	
	dvc = pcap_open_live(nic_name, 65535, 0, 1, err_buf);
	if (NULL == dvc) return -1;
	
	ret = pcap_compile(dvc, &fcode, pkt_filter, 1, 0);
	if (-1 == ret) return -2;
	
	ret = pcap_setfilter(dvc, &fcode);
	if (-1 == ret) return -3;
	
	pcap_loop(dvc, -1, get_pkt, dvc);
	
	return 0;
}
