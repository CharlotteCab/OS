#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  int pipefd[2];
  pipe(pipefd); //Creation du pipe
  if(!fork())
  {
    char s[16]="Hello World!";
    int a[2]; a[0]=5; a[1]=11;
    write(pipefd[1],s,16*sizeof(char)); //Ecriture dans le pipe
    write(pipefd[1],a,2*sizeof(int));
    return 0;
  }
  wait(NULL); //Attendre que le fils finisse

  char s[16];
  read(pipefd[0],s,16*sizeof(char)); //Lecture dans le pipe
  printf("%s\n",s);
  int a[2];
  read(pipefd[0],a,2*sizeof(int));
  printf("%d %d\n",a[0],a[1]);


  printf("Tube Nommés:\n");

  mkfifo("./fifo",0660); //Creation du fichier fifo
  int fd=open("./fifo",O_RDWR);

  if(!fork())
  {
    char s[16]="Hello World!";
    int a[2]; a[0]=6; a[1]=12;
    write(fd,s,16*sizeof(char)); //Ecriture dans le fichier
    write(fd,a,2*sizeof(int));
    return 0;
  }
  wait(NULL); //Attendre que le fils finisse
  char s2[16];
  read(fd,s2,16*sizeof(char)); //Lecture dans le fichier
  printf("%s\n",s2);
  read(fd,a,2*sizeof(int));
  printf("%d %d\n",a[0],a[1]);

  close(fd);

  return 0;
}
