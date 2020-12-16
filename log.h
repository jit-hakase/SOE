#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>
#include <errno.h>

/*
#define INF(fmt, ...) printf("[INF]: " fmt "\n", ##__VA_ARGS__)
#define TRC(fmt, ...) printf("[TRC]: " fmt "\n", ##__VA_ARGS__)
#define WRN(fmt, ...) printf("[WRN]: " fmt "\n", ##__VA_ARGS__)
#define ERR(fmt, ...) printf("[ERR]: " fmt "\n", ##__VA_ARGS__)
#define DIE(fmt, ...) printf("[DIE]: " fmt "\n", ##__VA_ARGS__), exit(errno)
*/

inline void LOG(const char *lvl, const char *func, int line, const char *fmt, ...)
{
	static pthread_mutex_t log_mtx = PTHREAD_MUTEX_INITIALIZER;
	static struct timespec ts;
	static struct tm time_ds;
	static char time_buf[20];
	
	pthread_mutex_lock(&log_mtx);
	
	clock_gettime(CLOCK_REALTIME, &ts);
	localtime_r(&ts.tv_sec, &time_ds);
	
	snprintf(time_buf, 20, "%04d-%02d-%02d %02d:%02d:%02d.%lu",
		time_ds.tm_year + 1900, time_ds.tm_mon + 1, time_ds.tm_mday,
		time_ds.tm_hour, time_ds.tm_min, time_ds.tm_sec, ts.tv_nsec);
	
	printf("[%s][%10.10s][%04d][%06d][%06lu][%s]: ",
		time_buf, func, line, getpid(), syscall(__NR_gettid), lvl);
	
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	printf("\n");
	
	pthread_mutex_lock(&log_mtx);
}

#define INF(fmt) LOG("INF", __FUNCTION__, __LINE__, fmt)
#define TRC(fmt) LOG("TRC", __FUNCTION__, __LINE__, fmt)
#define WRN(fmt) LOG("WRN", __FUNCTION__, __LINE__, fmt)
#define ERR(fmt) LOG("ERR", __FUNCTION__, __LINE__, fmt)
#define DIE(fmt) LOG("DIE", __FUNCTION__, __LINE__, fmt), exit(errno)

#endif
