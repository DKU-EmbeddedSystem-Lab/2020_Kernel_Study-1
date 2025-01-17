#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/common_threads.h"

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t {
	sem_t lock;
	sem_t writelock;
	sem_t turnstile;
	int readers;
	int writer_w;


} rwlock_t;


void rwlock_init(rwlock_t *rw) {
	Sem_init(&(rw->lock), 1);
	Sem_init(&(rw->writelock),1);
	Sem_init(&(rw->turnstile),1);
	rw->readers =0;
	rw->writer_w = 0;

}

void rwlock_acquire_readlock(rwlock_t *rw) {
	Sem_wait(&(rw->turnstile));
	Sem_post(&(rw->turnstile));

		Sem_wait(&(rw->lock));
		rw->readers++;
		if(rw->readers == 1)
			Sem_wait(&(rw->writelock));
		//sleep(1);
		Sem_post(&(rw->lock));
}

void rwlock_release_readlock(rwlock_t *rw) {
	Sem_wait(&(rw->lock));
	rw->readers--;
	if(rw->readers == 0)
		Sem_post(&(rw->writelock));
	Sem_post(&(rw->lock));
}

void rwlock_acquire_writelock(rwlock_t *rw) {
	Sem_wait(&(rw->turnstile));
	Sem_wait(&(rw->writelock));
	rw->writer_w++;
	//sleep(1);
}

void rwlock_release_writelock(rwlock_t *rw) {
	rw->writer_w--;
	Sem_post(&(rw->turnstile));
	Sem_post(&(rw->writelock));
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);fflush(NULL);
	for(int i =0; i < 10000; i++);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);fflush(NULL);
	for(int i =0; i < 10000; i++);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("rw->writer_w %d\n",(&lock)->writer_w);
    printf("end: value %d\n", value);

    return 0;
}

