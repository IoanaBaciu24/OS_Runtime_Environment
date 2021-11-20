



#include "tasks_implem.h"
#include "tasks_queue.h"
#include "threads_pool.h"
#include "debug.h"

tasks_queue_t *tqueue= NULL;


void create_queues(void)
{
    tqueue = create_tasks_queue();
}

void delete_queues(void)
{
    free_tasks_queue(tqueue);
}

void create_thread_pool(void)
{
    threads_init() ;

}

void dispatch_task(task_t *t)
{
    enqueue_task(tqueue, t);
}

task_t* get_task_to_execute(void)
{
    return dequeue_task(tqueue);
}

unsigned int exec_task(task_t *t)
{
    t->step++;
    t->status = RUNNING;

    PRINT_DEBUG(10, "Execution of task %u (step %u)\n", t->task_id, t->step);

    unsigned int result = t->fct(t, t->step);

    return result;
}

void terminate_task(task_t *t)
{
    t->status = TERMINATED;
    pthread_mutex_lock(&task_counter_lock);
    sys_finished.task_counter++;

    if(sys_finished.task_counter == sys_submitted.task_counter){
    printf("sys_submitted.task_counter %ld , sys_finished.task_counter %ld \n",sys_submitted.task_counter, sys_finished.task_counter );
    pthread_cond_signal(&task_count_cv);

    }

    pthread_mutex_unlock(&task_counter_lock);


    PRINT_DEBUG(10, "Task terminated: %u\n", t->task_id);

#ifdef WITH_DEPENDENCIES
    if(t->parent_task != NULL){
        task_t *waiting_task = t->parent_task;
        waiting_task->task_dependency_done++;
        //pthread_cond_broadcast(&(waiting_task->YUNA));

        task_check_runnable(waiting_task);
    }
#endif

}

void task_check_runnable(task_t *t)
{
#ifdef WITH_DEPENDENCIES
       //wait_subtask(t);
    pthread_mutex_lock(&(t->MOMO));
    if((t->task_dependency_done == t->task_dependency_count) && (t->status == WAITING )){
        t->status = READY;
        dispatch_task(t);
    }
    pthread_mutex_unlock(&(t->MOMO));
#endif
}
