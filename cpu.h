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

void atomic_initlock(volatile int *flag)
{
	__sync_lock_test_and_set(flag, 0);
}

void atomic_trylock(volatile int *flag)
{
	return __sync_fetch_and_or(flag, 1);
}

void atomic_idlelock(volatile int *flag)
{
	while (!atomic_trylock()) {
		sched_yield();
	}
}

void atomic_lock(volatile int *flag)
{
	while (!atomic_trylock()) {
		;
	}
}

void atomic_unlock(volatile int *flag)
{
	__sync_and_and_fetch(flag, 0);
}

#endif
