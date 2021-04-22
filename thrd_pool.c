#include <stdlib.h>
#include <pthread.h>

typedef struct {
	void * (*func)(void*);
	void *args;
} thrd_task_t;

typedef struct {
	int stat;
	size_t size;
	pthread_t *thrds;
	pthread_mutex_t task_mtx;
	pthread_cond_t task_cond;
} thrd_pool_t;

static void *thrd_task_routine(void *args)
{
	thrd_pool_t *pool;
	thrd_task_t *task;
	
	pool = args;
	for (;;) {
		pthread_mutex_lock(&pool->task_mtx);
		if (!q.empty()) {
			task = q.front();
			q.pop();
			pthread_mutex_unlock(&pool->task_mtx);
			task->func(task->args);	
		} else if (pool->stat == 0) {
			pthread_mutex_unlock(&pool->task_mtx);
			break;
		} else {
			pthread_cond_wait(&pool->task_cond, &pool->task_mtx);
			pthread_mutex_unlock(&pool->task_mtx);
		}
	}
	pthread_exit(NULL);
	return NULL;
}

thrd_pool_t* thrd_pool_init(size_t size)
{
	int ret, idx;
	thrd_pool_t *pool;
	
	pool = malloc(sizeof(thrd_pool_t));
	pool->stat = 1;
	pool->size = size;
	pool->thrds = malloc(sizeof(pthread_t) * pool->size);
	ret = pthread_mutex_init(&pool->task_mtx, NULL);
	if (ret == -1) return NULL; 
	ret = pthread_cond_init(&pool->task_cond, NULL);
	if (ret == -1) return NULL;
	
	for (idx = 0; idx < pool->size; ++idx) {
		ret = pthread_create(&pool->thrds[idx], NULL, thrd_task_routine, pool);
		if (-1 == ret) return NULL;
	}
	return pool;
}

void thrd_pool_destroy(thrd_pool_t *pool)
{
	int idx;
	
	pthread_mutex_lock(&pool->task_mtx);
	pool->stat = 0;
	pthread_mutex_unlock(&pool->task_mtx);
	pthread_cond_broadcast(&pool->task_cond);
	for (idx = 0; idx < pool->size; ++idx) {
		pthread_join(pool->thrds[idx], NULL);
	}
	pthread_mutex_destroy(&pool->task_mtx);
	pthread_cond_destroy(&pool->task_cond);
	free(pool->thrds);
	free(pool);
}

void thrd_pool_add_task(thrd_pool_t *pool, thrd_task_t *task)
{
	pthread_mutex_lock(&pool->task_mtx);
	q.push(task);
	pthread_cond_signal(&pool->task_cond);
	pthread_mutex_unlock(&pool->task_mtx);
}
