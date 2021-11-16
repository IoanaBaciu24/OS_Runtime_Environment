#ifndef __THREADS_POOL_H__
#define  __THREADS_POOL_H__

#include <pthread.h>


#include <unistd.h>

typedef struct threads {
  pthread_mutex_t lock ; // lock
  pthread_cond_t  nonempty ;
  pthread_cond_t  nonfull ;
  tasks_queue_t* queue ;  // queue
} threads_pool ;

threads_pool * threads_init(tasks_queue_t * tqueue) ;

void consume() ;









#endif
