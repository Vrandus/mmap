/***********************************************************************/
/********** Author: Minas Spetsakis                           **********/
/********** Description: A simple example of mmap usage       **********/
/********** for counting newlines in a file                   **********/
/***********************************************************************/
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int my_cnt(char *f)
{
  int fd, len, i, cnt;
  char *s;
  struct stat sb;

  if ((fd=open(f,O_RDONLY))<0)
    {
      printf("my_cnt: Could not open file\n");
      exit(1);
    }
  if (fstat(fd, &sb) == -1)
    {
      printf("my_cnt: Could not fstat file\n");
      exit(1);
    }
  
  len = sb.st_size;
  printf("length of file: %d\n",len);
  s = (char*)mmap(NULL,len,PROT_READ, MAP_SHARED, fd, 0);
  if (s==MAP_FAILED)
    {
      printf("my_cnt: Could not mmap file\n");
      exit(1);
    }
  close(fd);
  cnt = 0;
  for (i=0;i<len; i++)
    {
      if (s[i]=='\n') cnt++;
    }
  munmap(s,len);
  return cnt;
}