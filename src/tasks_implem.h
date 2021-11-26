#ifndef __TASKS_IMPLEM_H__
#define __TASKS_IMPLEM_H__

#include "tasks_types.h"
#include "threads_pool.h"
#include "debug.h"
#include "tasks_queue.h"


void create_queues(void);
void delete_queues(void);

void create_thread_pool(void);

void dispatch_task(task_t *t, int thread_id);
task_t* get_task_to_execute(int idx);
unsigned int exec_task(task_t *t);
void terminate_task(task_t *t);

void task_check_runnable(task_t *t);
void print_status(task_status_t t);
// extern queues_t *head ;

#endif
