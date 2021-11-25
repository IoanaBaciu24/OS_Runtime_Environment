



#include "tasks_implem.h"
#include "threads_pool.h"
#include "debug.h"
#include <stdlib.h>

//tasks_queue_t *tqueue= NULL;
queues_t *head = NULL;


void create_queues(void)
{
    // printf("before malloc\n");
    head = malloc(sizeof(queues_t));
    
    head->list = malloc(THREAD_COUNT*sizeof(tasks_queue_t*));
  
    for(int i=0;i<THREAD_COUNT;i++)
        head->list[i] = create_tasks_queue();
    head->index = 0;
    pthread_mutex_init(&(head->lock), NULL )  ;
}

void delete_queues(void)
{
    // free_tasks_queue(tqueue);
}

void create_thread_pool(void)
{
    threads_init() ;
    //   printf("BONJOUR MES ENFANTS\n");


}

void dispatch_task(task_t *t)
{
   pthread_mutex_lock(&(head->lock));
//    printf("enqueue at %d\n", head->index);
   enqueue_task(head->list[head->index], t);
   head->index = (head->index+1)%THREAD_COUNT;
   pthread_mutex_unlock(&(head->lock));


}


task_t* get_task_to_execute(int idx)
{
//     printf("salaaaaaam\n");
//    printf("dequeue at %d\n", idx);

    task_t *t = dequeue_task(head->list[idx]);
 

   return t;

}
void print_status(task_status_t st)
{
    if(st == WAITING) printf("WAITING\n");
    if(st == READY) printf("READY\n");
    if(st == RUNNING) printf("RUNNING\n");
    if(st == TERMINATED) printf("TERMINATED\n");

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
    //printf("sys_submitted.task_counter %ld , sys_finished.task_counter %ld \n",sys_submitted.task_counter, sys_finished.task_counter );
    pthread_cond_signal(&task_count_cv);

    }

    pthread_mutex_unlock(&task_counter_lock);


    PRINT_DEBUG(10, "Task terminated: %u\n", t->task_id);

#ifdef WITH_DEPENDENCIES
    if(t->parent_task != NULL){
        task_t *waiting_task = t->parent_task;
        print_status(waiting_task->status);
        pthread_mutex_lock(&(waiting_task->MOMO));

        waiting_task->task_dependency_done++;
        //pthread_cond_broadcast(&(waiting_task->YUNA));


        pthread_mutex_unlock(&(waiting_task->MOMO));

        task_check_runnable(waiting_task);

    }
#endif

}



void task_check_runnable(task_t *t)
{
#ifdef WITH_DEPENDENCIES
       //wait_subtask(t);
    //pthread_mutex_lock(&(t->MOMO));
    pthread_mutex_lock(&(t->MOMO));
    while ( t->status != WAITING) {

        printf("going to sleep %d\n", t->task_id);
        print_status(t->status);
      pthread_cond_wait(&(t->YUNA),&(t->MOMO));
    }
    if((t->task_dependency_done == t->task_dependency_count) && (t->status == WAITING )){
        t->status = READY;
          printf("task %d, task_depe_done %d  task_dep_count %d\n", t->task_id, t->task_dependency_done, t->task_dependency_count );
        
        dispatch_task(t);
    }
    pthread_mutex_unlock(&(t->MOMO));
    //pthread_mutex_unlock(&(t->MOMO));
#endif
}
