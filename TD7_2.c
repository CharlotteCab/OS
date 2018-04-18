#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc,char** argv)
{
  pid_t *t=malloc(atoi(argv[1])*sizeof(pid_t));
  int j;
  srand(time(NULL));

  for(int i=0;i<atoi(argv[1]);i++)
  {
    j=fork();
    if(!j)
    {
      printf("hey\n");
      sleep(rand()%10+1);
      i=atoi(argv[1]);
    }
    t[i]=j;
  }


  return 0;
}
