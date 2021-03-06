#include <stdlib.h>

#include "threads_pool.h"
#include <stdio.h>



void threads_init(tasks_queue_t * tqueue){

  pthread_t tid;
  int *idx;
  thread_id = -1;
  for ( int i = 0 ; i<THREAD_COUNT ; i ++) {
      idx = malloc(sizeof(int));
      *idx=i;
    
    pthread_create (  &tid, NULL , (void *)consume , idx ) ;

  }

}

void consume( void *args ){

  int idx = *((int *)args);
  thread_id = idx;

  while (1){
    
    active_task = get_task_to_execute(idx) ;


    task_return_value_t ret = exec_task( active_task ) ;

    if ( ret == TASK_COMPLETED ){

      terminate_task(active_task) ;
      

    }
    #ifdef WITH_DEPENDENCIES
    else{
      pthread_mutex_lock(&(active_task->MOMO));
      active_task->status = WAITING;
      pthread_cond_broadcast(&(active_task->YUNA));
      pthread_mutex_unlock(&(active_task->MOMO));
    }
    #endif

  }



}
