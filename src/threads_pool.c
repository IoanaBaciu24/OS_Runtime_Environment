#include <stdlib.h>
#include <.h>
#include "threads_pool.h"



threads_pool * threads_init(tasks_queue_t * tqueue){
  threads_pool * p = (threads_pool * ) malloc(sizeof(threads_pool));
  pthread_mutex_init(&p->lock, NULL )  ; // lock
  p->nonempty = PTHREAD_COND_INITIALIZER;
  p->nonfull = PTHREAD_COND_INITIALIZER;
  p->queue = tqueue ;  // queue
  for ( int i = 0 ; i<THREAD_COUNT ; i ++) {
    pthread_create ( NULL , NULL , consume , (void * ) p ) ;
  }
}

void consume( void * p ){
  threads_pool *pool = (  threads_pool * ) p ;
  while (1){
    pthread_mutex_lock( &(pool->lock) ) ;

    while ( pool -> queue -> index == 0 ) {
      pthread_cond_wait( &(pool -> nonempty ) , &(pool->lock) )  ;
    }

    task_t * t = get_task_to_execute() ;
    pthread_cond_signal ( &(pool->nonfull)) ;
    pthread_mutex_unlock(&(pool->lock)) ;
    task_return_value_t ret = exec_task( t ) ;
    if ( ret == TASK_COMPLETED ){
      terminate_task(t) ;
    }

    #ifdef WITH_DEPENDENCIES
            else{
                active_task->status = WAITING;
            }
    #endif


  }



}
