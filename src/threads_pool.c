#include <stdlib.h>

#include "threads_pool.h"
#include <stdio.h>


void threads_init(tasks_queue_t * tqueue){

  pthread_t tid;
  for ( int i = 0 ; i<THREAD_COUNT ; i ++) {
    pthread_create (  &tid, NULL , (void *)consume , NULL ) ;
  }

}

void consume(  ){

  while (1){

    active_task = get_task_to_execute() ;

    task_return_value_t ret = exec_task( active_task ) ;



    if ( ret == TASK_COMPLETED ){

      //printf("before terminate task in thread pool\n");
      terminate_task(active_task) ;
      active_task = NULL;
      //printf("after terminate task in thread pool\n");

    }

    #ifdef WITH_DEPENDENCIES
    else{
      active_task->status = WAITING;
      //printf("I AM IN THREAD POOL \n");
    }
    #endif

  }



}
