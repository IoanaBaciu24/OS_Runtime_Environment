#include <stdio.h>
#include <stdlib.h>
#include "tasks_queue.h"

queues_t *head = NULL;

tasks_queue_t* create_tasks_queue()
{
    tasks_queue_t *q = (tasks_queue_t*) malloc(sizeof(tasks_queue_t));

    q->task_buffer_size = QUEUE_SIZE;
    q->task_buffer = (task_t**) malloc(sizeof(task_t*) * q->task_buffer_size);
    q->index = 0;
    pthread_mutex_init(&(q->lock), NULL )  ;
    pthread_cond_init(&(q->nonempty), NULL);
    pthread_cond_init(&(q->nonfull), NULL);


    return q;
}

int resize_queue ( tasks_queue_t *q ) {

  task_t ** new  = (task_t**) realloc(q->task_buffer , 2 * sizeof(task_t*) * q->task_buffer_size);
  if (new) {
    //free(q->task_buffer);
    q->task_buffer = new ;
    q->task_buffer_size = 2 * q->task_buffer_size ;
    return 1 ;
  }
  return 0 ;

}

void free_tasks_queue(tasks_queue_t *q)
{
    /* IMPORTANT: We chose not to free the queues to simplify the
     * termination of the program (and make debugging less complex) */

    /* free(q->task_buffer); */
    /* free(q); */
}


void shift1(task_t **tsk_buf, int size)
{
    for(int i=1;i<size;i++)
    {
        tsk_buf[i-1] = tsk_buf[i];
    }
}

task_t * steal(int rando)
{
    if(rando == thread_id)
        return NULL;
    task_t *tsk = NULL;
    pthread_mutex_lock(&(head->list[rando]->lock));
    if(head->list[rando]->index >=1)
    {
        tsk = head->list[rando]->task_buffer[0];
        shift1(head->list[rando]->task_buffer, head->list[rando]->index);
        head->list[rando]->index--;

    }
    pthread_mutex_unlock(&(head->list[rando]->lock));
    return tsk;

 
}

void enqueue_task(tasks_queue_t *q, task_t *t)
{

      pthread_mutex_lock( &(q->lock)) ;
     while ( q -> task_buffer_size == q->index ) {

      
      int res = resize_queue(q);
      if(res == 0){
        printf("out of memory\n");
        exit(1);
      }
    }
    if ( !t ){
      printf("OH LALALALALALLAAAAAA \n");
    }
    q->task_buffer[q->index] = t;
    q->index++;
    pthread_cond_signal ( &(q->nonempty)) ;
    pthread_mutex_unlock(&(q->lock)) ;
}


task_t* dequeue_task(tasks_queue_t *q)
{
 
    pthread_mutex_lock( &(q->lock)) ;

     while ( q -> index == 0 ) {
         task_t  *t = steal(rand_generator_integer(0, THREAD_COUNT));
         if(t == NULL)
             pthread_cond_wait( &(q -> nonempty ) , &(q->lock) )  ;
        else
         {
          
            pthread_mutex_unlock(&(q->lock)) ;
            return t;
             
             }
    }
    task_t * t =  q->task_buffer[q->index-1];
    q->index--;
    pthread_cond_signal ( &(q->nonfull)) ;
    pthread_mutex_unlock(&(q->lock)) ;
    return t;
}
