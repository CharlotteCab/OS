#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>

int main(int argc, char** argv)
{
  clock_t t;
  t=times(NULL);

  struct tms buf; //En utilisant le buffer

  if(!fork())
  {
    char* args[4];
    args[0]="/bin/ls";
    args[1]="-R";
    args[2]=argv[1]; //Sans arguments prend le repertoire courrant
    args[3]=NULL;

    execv(args[0],args); //commande en 0, liste des arguments sans le 0 ?
  }
  else
  {
    wait(NULL);
    t=times(NULL)-t;
    times(&buf);
    clock_t T=buf.tms_cutime+buf.tms_cstime; //temps sys+user du fils
    double t1=(double)t/sysconf(_SC_CLK_TCK); //avoir le temps en s, en divisant par le nbr de cycle par s
    double t2=(double)T/sysconf(_SC_CLK_TCK);
    printf("\n");
    printf("clock:%lf et buf_clock:%lf\n",t1,t2);
    printf("Nbr de cycles par s:%ld\n",sysconf(_SC_CLK_TCK));
  }

  return 0;
}
