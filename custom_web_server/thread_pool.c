#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "thread_pool.h"
#include "parser.h"
#include "file_io.h"

//This is the infinite loop every thread runs
void *worker_thread(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;

    while (1) {
        //Locks the queue 
        pthread_mutex_lock(&pool->queue_mutex);

        //If queue is empty, sleep (this automatically unlocks the mutex while sleeping)
        while (pool->queue_head == NULL && !pool->stop_pool) {
            pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
        }

        if (pool->stop_pool) {
            pthread_mutex_unlock(&pool->queue_mutex);
            break;
        }

        //Grabs the first job from the queue
        job_t *job = pool->queue_head;
        pool->queue_head = job->next;
        if (pool->queue_head == NULL) {
            pool->queue_tail = NULL;
        }

        //Unlocks the queue so other threads can grab jobs
        pthread_mutex_unlock(&pool->queue_mutex);

        //Extracts socket and free memory
        int client_socket = job->client_socket;
        free(job);

    
        char buffer[4096] = {0};
        read(client_socket, buffer, sizeof(buffer) - 1);
        
        HttpRequest request;
        if (parse_http_request(buffer, &request) == 0) {
            serve_file(client_socket, request.path);
        }
        close(client_socket);
    }
    return NULL;
}

//Boots up the workers
thread_pool_t *thread_pool_init(int num_threads) {
    thread_pool_t *pool = malloc(sizeof(thread_pool_t));
    pool->num_threads = num_threads;
    pool->stop_pool = 0;
    pool->queue_head = NULL;
    pool->queue_tail = NULL;

    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond, NULL);
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }

    printf("[THREAD POOL] Successfully spawned %d worker threads.\n", num_threads);
    return pool;
}

//Adds a socket to the queue
void thread_pool_add_job(thread_pool_t *pool, int client_socket) {
    job_t *new_job = malloc(sizeof(job_t));
    new_job->client_socket = client_socket;
    new_job->next = NULL;

    pthread_mutex_lock(&pool->queue_mutex);
    
    if (pool->queue_tail == NULL) {
        pool->queue_head = new_job;
        pool->queue_tail = new_job;
    } else {
        pool->queue_tail->next = new_job;
        pool->queue_tail = new_job;
    }

    //wakes a sleeping thread
    pthread_cond_signal(&pool->queue_cond);
    pthread_mutex_unlock(&pool->queue_mutex);
}
