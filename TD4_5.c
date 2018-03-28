#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main()
{
  pid_t f;
  if(!(f=fork()))
  {
    for(size_t i=0;i<5;i++)
    {
      printf("%ld Je suis le fils.\n",i);
      sleep(1);
    }
  }
  else
  {
    sleep(3);
    kill(f,SIGSTOP);
    sleep(5);
    kill(f,SIGCONT);
  }

  return 0;
}
