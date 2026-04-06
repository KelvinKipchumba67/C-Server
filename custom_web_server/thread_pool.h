#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

//linked list node holding a client's socket
typedef struct job {
    int client_socket;
    struct job *next;
} job_t;

//master structure managing workers and the queue
typedef struct {
    pthread_t *threads;          
    int num_threads;             

    job_t *queue_head;           
    job_t *queue_tail;           

    pthread_mutex_t queue_mutex; //clock
    pthread_cond_t queue_cond;   //alarm clock

    int stop_pool;               
} thread_pool_t;

thread_pool_t *thread_pool_init(int num_threads);
void thread_pool_add_job(thread_pool_t *pool, int client_socket);

#endif
