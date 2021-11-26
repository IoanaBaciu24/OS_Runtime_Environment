#include <stdlib.h>

#include "tasks.h"
#include "tasks_implem.h"
#include "debug.h"
#include "utils.h"

system_state_t sys_state;
system_state_t sys_finished;
system_state_t sys_submitted;
pthread_mutex_t task_counter_lock;
pthread_cond_t  task_count_cv = PTHREAD_COND_INITIALIZER;
__thread task_t *active_task;
__thread int thread_id;


void runtime_init(void)
{
    /* a random number generator might be useful towards the end of
       the lab */
    rand_generator_init();

    create_queues();
    
    create_thread_pool();

    sys_state.task_counter = 0;
    sys_submitted.task_counter = 0;
    sys_finished.task_counter = 0;
    pthread_mutex_init(&task_counter_lock, NULL );
}

void runtime_init_with_deps(void)
{
#ifndef WITH_DEPENDENCIES
    fprintf(stderr, "ERROR: dependencies are not supported by the runtime. This application cannot be executed\n");
    exit(EXIT_FAILURE);
#endif

    runtime_init();
}



void runtime_finalize(void)
{
    task_waitall();

    PRINT_DEBUG(1, "Terminating ... \t Total task count: %lu \n", sys_state.task_counter);

    delete_queues();
}


task_t* create_task(task_routine_t f)
{
    task_t *t = malloc(sizeof(task_t));


    t->task_id = ++sys_state.task_counter;
    t->fct = f;
    t->step = 0;

    t->tstate.input_list = NULL;
    t->tstate.output_list = NULL;

#ifdef WITH_DEPENDENCIES
    t->tstate.output_from_dependencies_list = NULL;
    t->task_dependency_count = 0;
    t->parent_task = NULL;
    pthread_cond_init(&(t->YUNA), NULL);
    pthread_mutex_init(&(t->MOMO), NULL )  ;
 
#endif

    t->status = INIT;

    PRINT_DEBUG(10, "task created with id %u\n", t->task_id);

    return t;
}

void submit_task(task_t *t)
{
    t->status = READY;
    

#ifdef WITH_DEPENDENCIES
    if(active_task != NULL){
        t->parent_task = active_task;
         pthread_mutex_lock( &(active_task->MOMO));
        active_task->task_dependency_count++;
        pthread_mutex_unlock(&(active_task->MOMO));
        PRINT_DEBUG(100, "Dependency %u -> %u\n", active_task->task_id, t->task_id);
    }
#endif

    pthread_mutex_lock(&task_counter_lock);
    sys_submitted.task_counter ++;
    pthread_mutex_unlock(&task_counter_lock);
    dispatch_task(t, thread_id);
}


void task_waitall(void)
{


    pthread_mutex_lock(&task_counter_lock);
    while(sys_submitted.task_counter != sys_finished.task_counter)
    {
        pthread_cond_wait(&task_count_cv, &task_counter_lock);
    }
    pthread_mutex_unlock(&task_counter_lock);


}


