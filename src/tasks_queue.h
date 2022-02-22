#ifndef __TASKS_QUEUE_H__
#define __TASKS_QUEUE_H__

#include "tasks.h"
#include "tasks_implem.h"

#include "utils.h"





typedef struct tasks_queue{
    task_t** task_buffer;
    unsigned int task_buffer_size;
    unsigned int index;
    pthread_mutex_t lock ; // lock
    pthread_cond_t  nonempty ;
    pthread_cond_t  nonfull ;
} tasks_queue_t;


typedef struct queues{

    pthread_mutex_t lock ;    
    tasks_queue_t **list;
    unsigned int index;

} queues_t;

tasks_queue_t* create_tasks_queue(void);
void free_tasks_queue(tasks_queue_t *q);

void enqueue_task(tasks_queue_t *q, task_t *t);
task_t* dequeue_task(tasks_queue_t *q);
extern queues_t *head;
#endif
