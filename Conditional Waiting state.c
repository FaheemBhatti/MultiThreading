#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 

typedef struct _s_val {int  start_value; } s_val;
s_val start_value;
int start_value_set = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *init_function(void* arg);
void *parallel_exec(void* arg);

main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, &init_function, NULL);
   pthread_create( &thread2, NULL, &parallel_exec, NULL);
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   exit(0);
}

void *init_function(void *arg)
{
  s_val temp = initial_computation();
 
  pthread_mutex_lock(&lock);
  start_value = temp;
  start_value_set = 1;
 
  pthread_cond_broadcast(&cond);
  pthread_mutex_unlock(&lock);
}

void *parallel_exec(void *arg)
{
   pthread_mutex_lock(&lock);
   while(start_value_set == 0)
   {  
      pthread_cond_wait(&cond, &lock);
   }
   s_val temp = start_value;
   parallel_computation(temp);
}
