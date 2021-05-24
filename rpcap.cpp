#include <pcap.h>

char ebuf[4096];
pcap_t *pcap_hdr = pcap_open_offline("sr.pcap", ebuf);
if (NULL == pcap_hdr) return -1;

struct pcap_pkthdr pkt_hdr;
uint32_t len;
const uint8_t *data;

for (;;) {
  data = pcap_next(pcap_hdr, &pkt_hdr);
  if (!data) break;
  len = pkt_hdr.len;
  /* data, len */
}
