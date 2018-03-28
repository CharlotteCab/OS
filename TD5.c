#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* Hello(void* arg)
{
  printf("Hello World !\n");

  pthread_exit(0);
}

void* Alea(void* arg)
{
  int* t=(int*) arg;

  printf("");

  pthread_exit(0);
}

int main()
{
  pthread_t tid=0;
  int s,res;

  pthread_create(&tid,NULL,Hello,NULL);

  s=pthread_join(&tid,&res);

  return 0;
}
