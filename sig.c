static struct sigaction g_segv_act;
static struct sigaction g_abrt_act;

extern volatile sig_atomic_t g_sig_stat;

void signal_back_trace()
{
	void *buf[1024];
	int idx, sz = backtrace(buf, 1024);
	char **strs = backtrace_symbols(buf, sz);
	
	if (NULL != strs) {
		for (idx = 0; idx < sz; ++idx) {
			printf("%s\n", strs[idx]);
		}
	}
}

void signal_handler(int sig)
{
	printf("SIG: recv sig [%d]\n", sig);
	
	if (sig == SIGSEGV) {
		signal_back_trace();
		g_segv_act.sa_handler(sig);
		exit(EXIT_FAILURE);
	} else if (sig == SIGABRT) {
		signal_back_trace();
		g_abrt_act.sa_handler(sig);
		exit(EXIT_FAILURE);
	}
	if (sig == SIGPIPE) return;
	
	sig_stat = sig;
}

void signal_register()
{
	struct sigaction sig_act, old_sig_act;
	sigemptyset(&act.sa_mask);
	sig_act.sa_handler = signal_handler;
	
	sigaction(SIGINT, &sig_act, NULL);
	sigaction(SIGTERM, &sig_act, NULL);
	sigaction(SIGPIPE, &sig_act, NULL);
	sigaction(SIGSEGV, &sig_act, &g_segv_act);
	sigaction(SIGABRT, &sig_act, &g_abrt_act);
}
