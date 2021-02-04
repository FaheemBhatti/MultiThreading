#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#define NUM_PHIL 5

pthread_mutex_t tools_locks[NUM_PHIL];

int exit_flag = 0;

void *worker(void *argc){
   int id = *((int *) argc);
   int leftGlove = id;
   int rightGlove = (id+1) % NUM_PHIL;
   int wrench = (id+2)% NUM_PHIL;

   while (exit_flag == 0)
   {
      /* Locking Right Glove*/
      pthread_mutex_trylock(&tools_locks[rightGlove]);
      printf("/nWorker got Right Golve" );
      fflush(stdout);

      /* Locking left Glove*/
      pthread_mutex_trylock(&tools_locks[leftGlove]);
      printf("\nworker got Left Golve" );
      fflush(stdout);

      /* Locking wrench*/
      pthread_mutex_trylock(&tools_locks[wrench]);
      printf("\nwoker got the wrench");
      fflush(stdout);

      printf("\nTightening %d\n", id);
      usleep(10);

      /* UnLocking Wrench*/
      pthread_mutex_trylock(&tools_locks[wrench]);
      printf("\nLeaving the wrench");
      fflush(stdout);
      
      /* UnLocking left Glove*/
      pthread_mutex_unlock(&tools_locks[leftGlove]);
      printf("\nLeaving left Golve" );
      fflush(stdout);

      /* UnLocking Right Glove*/
      pthread_mutex_unlock(&tools_locks[rightGlove]);
      printf("\nLeaving Right Golve" );
      fflush(stdout);

      printf("\nIdle %d\n", id);
      sleep(1);
   }
}

int main(int argc, char **argv) 
{
   int i = 0;

   pthread_t threads[NUM_PHIL];
   int ids[NUM_PHIL];

   for(i = 0; i < NUM_PHIL; i++)
   {
      pthread_mutex_init(&tools_locks[i], NULL);
   }

   for(i = 0; i < NUM_PHIL; i++)
   {
      ids[i] = i;
      pthread_create(&threads[i], NULL, worker, &ids[i]);
   }

   //wait for key press
   getchar();
   exit_flag = 1;

   for(i = 0; i < NUM_PHIL; i++) 
   {
      pthread_join(threads[i], NULL);
   }

   for(i = 0; i < NUM_PHIL; i++) 
   {
      pthread_mutex_destroy(&tools_locks[i]);
   }
   return EXIT_SUCCESS;
}
