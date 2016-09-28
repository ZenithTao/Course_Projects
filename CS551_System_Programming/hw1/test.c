#include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ar.h>
#include <time.h>
#include <utime.h>
#include <stdbool.h>
#include <dirent.h> 


char test[100];
int main(void)
{
  DIR           *d;
  struct dirent *dir;
  d = opendir(".");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      sprintf(test, "%s",dir->d_name);
      printf("%s\n", test);
    }

    closedir(d);
  }

  return(0);
}