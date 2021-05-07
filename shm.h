#include <sys/ipc.h>
#include <sys/shm.h>

void * shm_init(key_t key, uint64_t size)
{
	int shm_id, ret;
	void *addr;
	
	shm_id = shmget(key, 0, 0);
	if (-1 != shm_id || errno != ENOENT) {
		return NULL;
	}
	
	shm_id = shmget(key, size, IPC_CREAT | 0666);
	if (-1 == shm_id) return NULL;
	
	addr = shmat(shm_id, NULL, 0);
	if (addr == (void *) -1) return NULL;
	
	return addr;
}

int shm_exit(key_t key)
{
	int shm_id, ret;
	void *addr;
	
	shm_id = shmget(key, 0, 0);
	if (-1 == shm_id) {
		return -1;
	} else {
		ret = shmctl(shm_id, IPC_RMID, NULL);
		if (-1 == ret) {
			return -2;
		}
	}
	return 0;
}

void * shm_addr(key_t key)
{
	int shm_id;
	void *addr;
	
	shm_id = shmget(key, 0, 0);
	if (-1 == shm_id) return NULL;
	addr = shmat(shm_id, NULL, 0);
	if (addr == (void *) -1) return NULL;
	
	return addr;
}
