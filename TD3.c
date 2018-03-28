#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int affiche_type(const char* chemin)
{
  struct stat buff;
  if(stat(chemin,&buff)<0) return -1;
  unsigned long right;

  switch (buff.st_mode & S_IFMT)
  {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
  }

  right=(unsigned long)(buff.st_mode & ~S_IFMT);
  right<<=55;

  for(int i=0;i<9;i++)
  {
    if(right & ~(0x7FFFFFFFFFFFFFFF))
    {
      switch (i%3)
      {
        case 0: printf("r"); break;
        case 1: printf("w"); break;
        case 2: printf("x"); break;
      }
    }
    else printf("-");
    right <<=1;
  }
  printf("\n");

  return 0;
}

int main()
{
  return affiche_type("TD3.c");
}
