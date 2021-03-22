#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define STR_HDR(str) ((struct str_hdr_t*) (str-sizeof(struct str_hdr_t)))

struct str_hdr_t
{
	uint64_t len;
	char buf[];
};

typedef const char *str_t;

uint64_t str_get_len(str_t str)
{
	return STR_HDR(str)->len;
}

str_t str_init(str_t str, uint64_t len)
{
	struct str_hdr_t *pstr;
	
	pstr = malloc(sizeof(struct str_hdr_t) + len + 1);
	pstr->len = len;
	memcpy(pstr->buf, str, len);
	pstr->buf[len] = '\0';
	return pstr->buf;
}

str_t str_new(str_t str)
{
	return str_init(str, strlen(str));
}

void str_del(str_t str)
{
	free(STR_HDR(str));
}

str_t str_add_str(str_t str1, str_t str2)
{
	uint64_t len1, len2;
	struct str_hdr_t *pstr;
	
	len1 = strlen(str1);
	len2 = strlen(str2);
	
	pstr = malloc(sizeof(struct str_hdr_t) + len1 + len2 + 1);
	pstr->len = len1 + len2;
	memcpy(pstr->buf, str1, len1);
	memcpy(pstr->buf + len1, str2, len2);
	pstr->buf[len1 + len2] = '\0';
	return pstr->buf;
}
