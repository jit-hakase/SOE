#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define CACHE_LINE_SIZE 64

struct fifo_t {
	uint8_t pad0[CACHE_LINE_SIZE];
	uint64_t                front;
	uint8_t pad1[CACHE_LINE_SIZE];
	uint64_t                 rear;
	uint8_t pad2[CACHE_LINE_SIZE];
	uint64_t                  cap;
	uint64_t                 mask;
	uint64_t                 size;
	void                     *buf;
} __attribute__((aligned(CACHE_LINE_SIZE)));

struct fifo_t * fifo_init(size_t cap, size_t size)
{
	struct fifo_t *fifo;
	fifo = malloc(sizeof(struct fifo_t));
	fifo->front = fifo->rear = 0;
	fifo->cap = cap;
	fifo->mask = cap - 1;
	fifo->size = size;
	fifo->buf = malloc(cap * size);
	return fifo;
}

void fifo_destroy(struct fifo_t *fifo)
{
	free(fifo->buf);
	free(fifo);
}

static inline __attribute__((always_inline))
void fifo_push(struct fifo_t *fifo, void *buf, size_t size)
{
	static uint64_t seq;
re:
	seq = fifo->rear;
	
	if (seq < (__atomic_load_n(&fifo->front, __ATOMIC_ACQUIRE) + fifo->cap)) {
		memcpy(fifo->buf + (seq & fifo->mask) * fifo->size, buf, size);
		__atomic_store_n(&fifo->rear, fifo->rear+1, __ATOMIC_RELEASE);
	} else {
		CPU_PAUSE();
		goto re;
	}
}

static inline __attribute__((always_inline))
void fifo_pop(struct fifo_t *fifo, void *buf, size_t size)
{
	static uint64_t seq;
re:
	seq = fifo->front;
	
	if (seq < __atomic_load_n(&fifo->rear, __ATOMIC_ACQUIRE)) {
		memcpy(buf, fifo->buf + (seq & fifo->mask) * fifo->size, size);
		__atomic_store_n(&fifo->front, fifo->front+1, __ATOMIC_RELEASE);
	} else {
		CPU_PAUSE();
		goto re;
	}
}
