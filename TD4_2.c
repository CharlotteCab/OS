#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void fils()
{
  int rng=0;
  srand(time(NULL));
  rng=rand()%10+1;
  sleep(rng);
  pid_t id=getpid();
  printf("(fils) PID:%d.\n",(int)id);
}

int main()
{

  pid_t cpid;

  for(size_t i=0;i<10;i++)
  {
    cpid=fork();
    if(!cpid)
    {
      fils();
      i=11;
    }
    else
    {
      wait(NULL);
      printf("(pere) PID:%d.\n",cpid);
    }
  }
  return 0;
}
