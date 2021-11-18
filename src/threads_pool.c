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
  
    task_t * t = get_task_to_execute() ;

    task_return_value_t ret = exec_task( t ) ;
  
    if ( ret == TASK_COMPLETED ){
      printf("calling terminate task\n");
      terminate_task(t) ;
    }

    #ifdef WITH_DEPENDENCIES
            else{
                active_task->status = WAITING;
            }
    #endif


  }



}
