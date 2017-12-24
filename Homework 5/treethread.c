#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void * threadFunc(void* start);
void join (int num);
int Tthreads;
pthread_t * tid;

int main(int argc, char** argv)
{
  int nthreads;
  nthreads = atoi (argv[1]);
  Tthreads = (2 << (nthreads - 1)) - 2;

  if (argc != 2)
    {
      printf ("Need two arguments\n");
      exit (1);
    }

  if (nthreads < 1)
    {
      printf("Need positive argument\n");
      exit (1);
    }

  tid = (pthread_t*) malloc(sizeof(pthread_t) *(Tthreads + 1));

  int i;
  for (i = 0; i < Tthreads + 1; i++)
    {
      pthread_create(&tid[i], NULL, threadFunc, NULL);
    }
  join (0);
  exit (0);
}

void * threadFunc(void* start)
{
    return NULL;
}

void join (int num)
{
  if ((num * 2) < Tthreads)
    {
      join ((num * 2) + 1);
      join ((num * 2) + 2);
    }
  printf("Thread %i done\n", num);
  pthread_join(tid[num],NULL);
}
