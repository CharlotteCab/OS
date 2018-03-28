#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int nfoism(int n, int m)
{
  int cpt=0;

  for(int i=0;i<m;i++)
  {
    for(int j=0;j<n;j++)
    {
      if(!fork())
      {
        exit(3);
      }
      else
      {
        wait(NULL);
        cpt++;
      }
    }
  }
  return cpt;
}

int npuism(int n, int m)
{
  int cpt=0;
  int id=-1;
  //Creation Pere

  for(int i=0;i<m;i++)
  {
      if(!fork())
      {
        //Creation Fils
        for(int j=0;j<n;j++)
        {
          if(!fork())
          {
            //Creation Petit-Fils
            exit(3); //Termine les petits-fils
          }
          else
          {
            wait(NULL);
          }
          exit(3); //Termine les Fils
        }
      }
      else
      {
        wait(NULL);
        cpt+=n;
        id=getpid();
        printf("top pid %d\n",id);
      }
  }
  return cpt;
}

int npuissancem(int n, int m)
{
  int cpt=0;

  for(int i=0;i<m;i++)
  {
    if(!fork())
    {
      
    }
    exit(3);
  }
  return cpt;
}

int main(int argc, char** argv)
{
  int n=atoi(argv[1]);
  int m=atoi(argv[2]);
  int cpt=0;
  //cpt=nfoism(n,m);
  //cpt=npuism(n,m);
  cpt=npuissancem(n,m);

  printf("Nbr de processus: %d.\n",cpt);

  return 0;
}
