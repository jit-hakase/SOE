#ifndef ACL_H
#define ACL_H

#include <getopt.h>

typedef void (*kv_fn_t)(const char *, const char *);

void acl_parse(int argc, char **argv, kv_fn_t fn);

void acl_set(const char *key, const char *val);

#endif
