#include <stdint.h>
#include <malloc.h>
#include <stdio.h>

struct pcap_file_header_t
{
	uint32_t magic;
	uint16_t ver_major;
	uint16_t ver_minor;
	uint32_t zone;
	uint32_t sig_figs;
	uint32_t snap_len;
	uint32_t link_type;
};

struct pcap_header_t
{
	uint32_t timestamp_s;
	uint32_t timestamp_ms;
	uint32_t cap_len;
	uint32_t len;
};

int read_pcap(const char *file_name)
{
	struct pcap_file_header_t file_hdr;
	struct pcap_header_t pcap_hdr;
	int cnt = 0;
	size_t len;
	FILE *fp;
	void *buf;
	
	fp = fopen(file_name, "r");
	if (NULL == fp) return -1;
	
	len = fread(&file_hdr, sizeof(file_hdr), 1, fp);
	if (0 == len) {
		fclose(fp);
		return -2;
	}
	
	while (!feof(fp)) {
		len = fread(&pcap_hdr, sizeof(pcap_hdr), 1, fp);
		if (len == 0) {
			break;
		}
		
		buf = malloc(pcap_hdr.cap_len);
		len = fread(buf, 1, pcap_hdr.cap_len, fp);
		if (len != pcap_hdr.cap_len) {
			break;
		}
		
		/* ...parse data... */
		
		free(buf);
		++cnt;
	}
	fclose(fp);
	return cnt;
}
