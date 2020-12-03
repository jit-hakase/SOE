#include <def.h>
#include <acl.h>
#include <sig.h>

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

volatile sig_atomic_t g_sig_stat = 0;

void main_event_loop()
{
	
}

int main(int argc, char **argv) {
	signal_register();
	
	while (SIGTERM != g_sig_stat && SIGINT != g_sig_stat) {
		main_event_loop();
	}
	return 0;
}
