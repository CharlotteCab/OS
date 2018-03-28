#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int Affiche(const char * chemin)
{
	
	struct stat buf;
	if(stat(chemin, &buf) < 0 ) return -1;
	unsigned long raight;
	
	 switch (buf.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device ");            break;
           case S_IFCHR:  printf("character device ");        break;
           case S_IFDIR:  printf("directory ");               break;
           case S_IFIFO:  printf("FIFO/pipe ");               break;
           case S_IFLNK:  printf("symlink ");                 break;
           case S_IFREG:  printf("regular file ");            break;
           case S_IFSOCK: printf("socket" );                  break;
           default:       printf("unknown?" );                break;
           }
     

    raight = (unsigned long)(buf.st_mode & ~S_IFMT);
    raight <<= 55; //yup :)
    
    for(int i = 0; i < 9; i++)
    {
		if(raight & ~(0x7FFFFFFFFFFFFFFF))
		{
			switch(i%3)
			{
				case 0: printf("r"); break;
				case 1: printf("w"); break;
				case 2: printf("x"); break;
			 }
		}
		else printf("-");
		raight <<= 1;
	}
    printf(" ");
    
    
    printf("UID: %d GID: %d ", buf.st_uid, buf.st_gid);
    printf("Size: %ld octet ", buf.st_size);
    return 0;
}

void Flow_rep(const char* path)
{
	DIR*  rep = opendir(path);
	struct dirent* current_rep;
	int safe = 0;
	char path_cat[256];
		
	while( (current_rep = readdir(rep)) != NULL)
	{
		strcpy(path_cat, path);
		strncat(path_cat, "/", 1);
		
		strcat(path_cat, current_rep->d_name);
		//HOW DARE YOU
		//il est 00:28 donc strncat sera implémenté plus tard (jeudi probablement)
		
		printf("<%s>\n", path_cat);
		safe = Affiche(path_cat);
		if(safe < 0) exit(-1);
		//Affiche("/home/user/a/a.class");
		printf("%s\n", current_rep->d_name);
	}
	
}

int main()
{
	Flow_rep("/home/user/a");
	
	return 0;
}
