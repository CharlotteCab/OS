#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main()
{
  int rng=0;
  pid_t idf;
  int pipefd[2];
  char buf[2];

  srand(time(NULL));

  if(pipe(pipefd)==-1) exit(-1);

  if((idf=fork()))
  {
    printf("Hello(pere)\n");
    wait(NULL);
  }
  else
  {
    printf("World!(fils)\n");
  }

  if(idf)
  {
    printf("Mon PID est %d et celui de mon fils est %d.\n",getpid(),idf);
    read(pipefd[0],buf,sizeof(int));
    printf("(pere) Le nombre est %s.\n",buf);
  }
  else
  {
    printf("Mon PID est %d et celui de mon père est %d.\n",getpid(),getppid());
    rng=rand()%50+1;
    printf("(fils) Le nombre est %d.\n",rng);
    sprintf(buf,"%d",rng);
    write(pipefd[1],buf,sizeof(int));
  }

  return 0;
}
