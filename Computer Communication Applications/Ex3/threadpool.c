#include <malloc.h>
#include "threadpool.h"

#define TRUE 1
#define FALSE 0
#define FAILED -1
#define SUCCEEDED 1

work_t *dequeue(threadpool *threadPoolPointer);

int enqueue(threadpool *threadPoolPointer, work_t *newJobPointer);

// This function receives one argument: 1- the number of the threads in the pool
// the function creates the pool and initializes the threads pool variables
// in any case of failure it frees all the manually allocated memories then it returns
threadpool *create_threadpool(int num_threads_in_pool) {

    threadpool *threadPoolPointer = (threadpool *) malloc(sizeof(threadpool) * 1);

    if(threadPoolPointer == NULL){

        fprintf(stdout, "Allocation failure: malloc failed\n");
        return NULL;
    }

    threadPoolPointer->num_threads = num_threads_in_pool;
    threadPoolPointer->qsize = 0;
    threadPoolPointer->threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads_in_pool);

    if(threadPoolPointer->threads == NULL){

        fprintf(stdout, "Allocation failure: malloc failed\n");
        free(threadPoolPointer);
        return NULL;
    }
    threadPoolPointer->qhead = NULL;
    threadPoolPointer->qtail = NULL;
    threadPoolPointer->shutdown = FALSE;
    threadPoolPointer->dont_accept = FALSE;

    pthread_mutex_init(&(threadPoolPointer->qlock), NULL);
    pthread_cond_init(&(threadPoolPointer->q_not_empty), NULL);
    pthread_cond_init(&(threadPoolPointer->q_empty), NULL);

    int i;
    for (i = 0; i < num_threads_in_pool; i++) {

        pthread_create((threadPoolPointer->threads + i), NULL, do_work, (void *) threadPoolPointer);
    }

    return threadPoolPointer;
}

// This function receives one argument: 1- a pointer to the thread pool
// each thread applies this function, and it live there handling clients
// by taking each time a job from the jobs queue and making that job
// if there is no job the thread waits till the dispatch function awakes him after adding a new job
// the thread lock and unlock the mutexes according to the need, so that the other thread don't read or write to the queue which causes a problems [synchronization]
void *do_work(void *p) {

    threadpool *threadPoolPointer = (threadpool *) p;

    while (TRUE) {

        pthread_mutex_lock(&(threadPoolPointer->qlock));

        if (threadPoolPointer->shutdown == TRUE) {
            pthread_mutex_unlock(&(threadPoolPointer->qlock));
            return NULL;
        }

        if (threadPoolPointer->qsize == 0) {

            while (threadPoolPointer->qsize == 0 && threadPoolPointer->shutdown == FALSE) {
                pthread_cond_wait(&(threadPoolPointer->q_not_empty), &(threadPoolPointer->qlock));
            }

            if (threadPoolPointer->shutdown == TRUE) {
                pthread_mutex_unlock(&(threadPoolPointer->qlock));
                return NULL;
            }
        }

        work_t *currentJob = dequeue(threadPoolPointer);

        if (threadPoolPointer->dont_accept == TRUE && threadPoolPointer->qsize == 0) {
            pthread_cond_signal(&(threadPoolPointer->q_empty));
        }

        pthread_mutex_unlock(&(threadPoolPointer->qlock));

        currentJob->routine(currentJob->arg);

        free(currentJob);
    }

}

// This function receives three arguments: 1- a pointer for the thread pool, 2- a pointer to the function that the thread going to apply
// the function creates a new work_t struct which holds the clientHandler function and the argument for that function which is the client SD
// the function adds that job to the jobs queue then it wakes up a thread to make the job
// the function lock and unlock the mutex according to the need so that the other thread don't read or write to the queue which causes a problems [synchronization]
void dispatch(threadpool *from_me, dispatch_fn dispatch_to_here, void *arg) {

    if (from_me->dont_accept == TRUE) {
        return;
    }

    work_t *newJobPointer = (work_t *) malloc(sizeof(work_t) * 1);

    if(newJobPointer == NULL){
        fprintf(stdout, "Allocation failure: malloc failed\n");
        return;
    }

    newJobPointer->routine = dispatch_to_here;
    newJobPointer->arg = arg;
    newJobPointer->next = NULL;

    pthread_mutex_lock(&(from_me->qlock));

    enqueue(from_me, newJobPointer);

    pthread_cond_signal(&(from_me->q_not_empty));

    pthread_mutex_unlock(&(from_me->qlock));

}

// This function receives one argument: 1- a pointer to the thread pool
// the function destroys the thread pool starting by turning on the flag dont_accept to tell the dispatch
// function not to add new jobs to the jobs queue if the jobs queue is not empty it waits till the queue
// is empty (the thread that takes the last job awakes it) then it turns on the flag shut down, and it awakes all the threads,
// so they see the shutdown flag on, and they exit after that it makes the needed frees and destroys for the locks
void destroy_threadpool(threadpool *destroyme) {

    destroyme->dont_accept = TRUE;

    pthread_mutex_lock(&(destroyme->qlock));

    if (destroyme->qsize != 0) {

        pthread_cond_wait(&(destroyme->q_empty), &(destroyme->qlock));
    }

    destroyme->shutdown = TRUE;

    pthread_mutex_unlock(&(destroyme->qlock));

    pthread_cond_broadcast(&(destroyme->q_not_empty));

    int i;
    for (i = 0; i < destroyme->num_threads; i++) {

        pthread_join(destroyme->threads[i], NULL);
    }

    pthread_mutex_destroy(&(destroyme->qlock));
    pthread_cond_destroy(&(destroyme->q_not_empty));
    pthread_cond_destroy(&(destroyme->q_empty));


    free(destroyme->threads);
    free(destroyme);
}

// This function receives two arguments: 1- a pointer for the thread pool, 2- a pointer of work_t which is the job that we want to add to the queue
// the function adds the new job to the jobs queue
int enqueue(threadpool *threadPoolPointer, work_t *newJobPointer) {

    if (threadPoolPointer == NULL || newJobPointer == NULL) {
        return FAILED;
    }

    if (threadPoolPointer->qtail == NULL) {

        threadPoolPointer->qhead = newJobPointer;
        threadPoolPointer->qtail = newJobPointer;
        threadPoolPointer->qsize = threadPoolPointer->qsize + 1;

        return SUCCEEDED;
    }

    threadPoolPointer->qtail->next = newJobPointer;
    threadPoolPointer->qtail = newJobPointer;
    threadPoolPointer->qsize = threadPoolPointer->qsize + 1;

    return SUCCEEDED;
}

// This function receives two arguments: 1- a pointer for the thread pool
// the function pops and returns the first job in the queue
work_t *dequeue(threadpool *threadPoolPointer) {

    if (threadPoolPointer == NULL || threadPoolPointer->qhead == NULL) {
        return NULL;
    }

    work_t *currentJob = threadPoolPointer->qhead;
    threadPoolPointer->qhead = threadPoolPointer->qhead->next;
    threadPoolPointer->qsize = threadPoolPointer->qsize - 1;

    if (threadPoolPointer->qsize == 0) {
        threadPoolPointer->qtail = NULL;
    }

    return currentJob;
}



