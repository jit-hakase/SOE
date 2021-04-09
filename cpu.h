#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <pthread.h>
#include <sched.h>

#define CPU_PAUSE() asm volatile (".byte 0xf3, 0x90")

#define RDTSCP(val) do { \
	uint32_t __a, __d, __c; \
	asm volatile ("rdtscp" : "=a" (__a), "=d" (__d), "=c" (__c)); \
	val = (uint64_t)(__a) | ((uint64_t)(__d) << 32); \
} while (0)

inline int cpu_bind_core(int cpu_id, int prior) {
	cpu_set_t mask;
	pthread_attr_t attr;
	sched_param param;
	
	if (-1 == cpu_id) return 0;
	
	CPU_ZERO(&mask);
	CPU_SET(cpu_id, &mask);
	
	if (0 != pthread_attr_init(&attr)) return -1;
	
	if (0 != pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask)) return -1;
	
	if (0 != pthread_attr_setschedpolicy(&attr, SCHED_RR)) return -1;
	
	param.__sched_priority = prior;
	
	if (0 != pthread_attr_setschedparam(&attr, &param)) return -1;
	
	return 0;
}

inline void lock_spin_lock(volatile uint32_t *flag)
{
	while (__sync_lock_test_and_set(flag, 1)) {
		while (*flag) {
			__asm__("pause" ::: "memory");
		}
	}
}

inline void lock_spin_unlock(volatile uint32_t *flag)
{
	__sync_lock_release(flag);
}

#include <sys/resource.h>
#include <sys/time.h>

void generate_coredump()
{
	struct rlimit res;
	res.rlim_cur = RLIM_INFINITY;
	res.rlim_max = RLIM_INFINITY;
	setrlimit(RLIMIT_CORE, &res);
}

#endif
