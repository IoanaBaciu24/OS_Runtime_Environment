#include <stdio.h>
#include <stdlib.h>
#include "tasks_queue.h"



tasks_queue_t* create_tasks_queue()
{
    tasks_queue_t *q = (tasks_queue_t*) malloc(sizeof(tasks_queue_t));

    q->task_buffer_size = QUEUE_SIZE;
    q->task_buffer = (task_t**) malloc(sizeof(task_t*) * q->task_buffer_size);
    q->index = 0;
    pthread_mutex_init(&(q->lock), NULL )  ; 
    // q->nonempty = PTHREAD_COND_INITIALIZER;
    // q->nonfull = PTHREAD_COND_INITIALIZER;
    pthread_cond_init(&(q->nonempty), NULL);
    pthread_cond_init(&(q->nonfull), NULL);


    return q;
}


void free_tasks_queue(tasks_queue_t *q)
{
    /* IMPORTANT: We chose not to free the queues to simplify the
     * termination of the program (and make debugging less complex) */
    
    /* free(q->task_buffer); */
    /* free(q); */
}


void enqueue_task(tasks_queue_t *q, task_t *t)
{
    // if(q->index == q->task_buffer_size){
    //     fprintf(stderr,"ERROR: the queue of tasks is full\n");
    //     exit(EXIT_FAILURE);
    // }

    // q->task_buffer[q->index] = t;
    // q->index++;
      pthread_mutex_lock( &(q->lock)) ;
     while ( q -> task_buffer_size == q->index ) {
      pthread_cond_wait( &(q -> nonfull ) , &(q->lock) )  ;
    }
    q->task_buffer[q->index] = t;
    q->index++;
    pthread_cond_signal ( &(q->nonempty)) ;
    pthread_mutex_unlock(&(q->lock)) ;
}


task_t* dequeue_task(tasks_queue_t *q)
{
    // if(q->index == 0){
    //     return NULL;
    // }

    // task_t *t = q->task_buffer[q->index-1];
    // q->index--;

    // return t;
    pthread_mutex_lock( &(q->lock)) ;
     while ( q -> index == 0 ) {
      pthread_cond_wait( &(q -> nonempty ) , &(q->lock) )  ;
    }
    task_t * t =  q->task_buffer[q->index-1];
    q->index--;
    pthread_cond_signal ( &(q->nonfull)) ;
    pthread_mutex_unlock(&(q->lock)) ;
    return t;
}

