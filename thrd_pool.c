#include <stdlib.h>
#include <pthread.h>

struct thrd_task_t
{
	void * (*fn)(void*);
	void *args;
};

struct thrd_pool_t
{
	size_t stat;
	size_t max_thrd_num;
	pthread_t *thrds;
	struct thrd_task_t *task_hdr;
	pthread_cond_t que_rdy;
	pthread_mutex_t que_mtx;
};

static void *task_routine(void *args)
{
	struct thrd_pool_t *pool = args;
	struct thrd_task_t *task = NULL;
	
	while (1) {
		pthread_mutex_lock(&pool->que_mtx);
		while (!pool->task_hdr) {
			pthread_cond_wait(&pool->que_rdy, &pool->que_mtx);
		}
		
		if (!pool->stat) {
			pthread_mutex_unlock(&pool->que_mtx);
			break;
		}
		
		/* pop from data struct */
		
		pthread_mutex_unlock(&pool->que_mtx);
		
		task->fn(task->args);
		free(task);
	}
	return NULL;
}

int thrd_pool_init(thrd_pool_t **pool, size_t max_thrd_num, void* (*fn)(void*))
{
	int ret, idx;
	
	*pool = malloc(sizeof(struct thrd_pool_t));
	if (NULL == *pool) return -1;
	
	(*pool)->max_thrd_num = max_thrd_num;
	(*pool)->thrds = malloc(sizeof(pthread_t) * max_thrd_num);
	if (NULL == (*pool)->thrds) return -2;
	
	ret = pthread_mutex_init(&(*pool)->que_mtx, NULL);
	if (-1 == ret) return -3;
	
	ret = pthread_cond_init(&(*pool)->que_rdy, NULL);
	if (-1 == ret) return -4;
	
	for (idx = 0; idx < max_thrd_num; ++idx) {
		ret = pthread_create(&(*pool)->thrds[idx], NULL, fn, *pool);
		if (-1 == ret) return -5;
	}
	return 0;
}

int thrd_pool_destroy(thrd_pool_t *pool)
{
	int idx;
	
	if (NULL == pool) return -1;
	if (!pool->stat) return -2;
	
	pool->stat = 0;
	pthread_mutex_lock(&pool->que_mtx);
	pthread_cond_broadcast(&pool->que_rdy);
	pthread_mutex_unlock(&pool->que_mtx);
	
	for (idx = 0; idx < pool->max_thrd_num; ++idx) {
		pthread_join(pool->thrds[idx], NULL);
	}
	free(pool->thrds);
	
	/* free data struct */
	
	pthread_mutex_destroy(&pool->que_mtx);
	pthread_cond_destroy(&pool->que_rdy);
	free(pool);
	return 0;
}

int thrd_pool_add_task(thrd_pool_t *pool, void *(*fn)(void*), void *args)
{
	thrd_task_t *task;
	
	if (NULL == fn) return -1;
	
	task = malloc(sizeof(thrd_task_t));
	if (NULL == task) return -2;
	
	task->fn = fn;
	task->args = args;
	
	pthread_mutex_lock(&pool->que_mtx);
	
	/* push to data struct */
	
	pthread_cond_signal(&pool->que_rdy);
	pthread_mutex_unlock(&pool->que_mtx);
	return 0;
}
