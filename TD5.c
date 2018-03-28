#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

typedef struct Tableau
{
  int* t;
  int n;
}T;

void* Hello()
{
  printf("Hello World !\n");

  pthread_exit(NULL);
}

void* Alea(void* arg)
{
  int* t=(int*) arg;

  printf("Le nbr alea est: %d\n",t[0]);

  pthread_exit(NULL);
}

void* AleaThread(void* arg)
{
  int* t=(int*)arg;
  t[1]=rand()%100;

  pthread_exit(NULL);
}

void* Moyenne(void* arg)
{
  int moy;
  int somme=0;
  int* t=(int*)arg;

  for(int i=0;i<5;i++)
  {
    somme+=t[i];
  }
  moy=somme/5;

  printf("La Moyenne est:%d\n",moy);

  pthread_exit(NULL);
}

void* MoyenneN(void* arg)
{
  int moy;
  int somme=0;
  T *t=(T*)arg;

  for(int i=0;i<t->n;i++)
  {
    somme+=t->t[i];
  }
  moy=somme/t->n;

  printf("La MoyenneN est:%d\n",moy);

  pthread_exit(NULL);
}

void AleaTab(int* t,int n)
{
  for(int i=0;i<n;i++)
  {
    t[i]=rand()%100;
  }
}

int main()
{
  pthread_t tid[5];
  int s;
  int t[3],t1[5];
  T t2;
  t2.n=10;

  t2.t=malloc(t2.n*sizeof(int));
  srand(times(NULL));
  t[0]=rand()%100;

  AleaTab(t1,5);
  AleaTab(t2.t,t2.n);
  s=pthread_create(&tid[0],NULL,Hello,NULL);
  s=pthread_create(&tid[1],NULL,Alea,((void*)t));
  s=pthread_create(&tid[2],NULL,AleaThread,((void*)t));
  s=pthread_create(&tid[3],NULL,Moyenne,((void*)t1));
  s=pthread_create(&tid[4],NULL,MoyenneN,(&t2));

  s=pthread_join(tid[0],NULL);
  s=pthread_join(tid[1],NULL);
  s=pthread_join(tid[2],NULL);
  s=pthread_join(tid[3],NULL);
  s=pthread_join(tid[4],NULL);
  printf("Alea du Thread:%d\n",t[1]);

  free(t2.t);

  return 0;
}
