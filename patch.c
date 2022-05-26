#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <elf.h>

int main(int argc, char **argv)
{
  if (argc != 6)
  {
    fprintf(stderr, "(wrong usage )Please enter: Source file, Source position, size, destination file, destination position");
    return 0;
  }
  const char *address = NULL;
  struct stat stats;
  const char *address_2 = NULL;
  struct stat stats_2;
  int fd_source, fd_dest;
  int size = atoi(argv[3]);
  unsigned int s_pos,d_pos; 
  sscanf(argv[2],"%x",&s_pos);
  sscanf(argv[5],"%x",&d_pos);
  
  fd_source = open(argv[1], O_RDWR,0777);
  if (fstat(fd_source, &stats) != 0)
  {
    perror("error file size");
  }

  fd_dest = open(argv[4], O_RDWR,0777);
  if (fstat(fd_dest, &stats_2) != 0)
  {
    perror("error file size");
  }
  size_t filesize=size;
 
 


  fprintf(stderr, "values; fd_source %d fd_dest %d size %d s_pos 0x%x d_pos 0x%x\n", fd_source, fd_dest, size, s_pos, d_pos);
  address = mmap(NULL,stats.st_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd_source,0);
  if (address == MAP_FAILED)
  {
    perror("address map failed");
  }
    
  address_2 = mmap(NULL,stats_2.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dest,0);
  if (address_2 == MAP_FAILED)
  {
    perror("address_2 map failed");
  }
  char * src_char=(char *)address;
  char * dest_char=(char *)address_2;

   memcpy((void *)(dest_char+d_pos),(void *)(src_char+s_pos), filesize);

  if (address != MAP_FAILED)
  {
    munmap((void *)address, stats.st_size);
  }
  if (address_2 != MAP_FAILED)
  {
    munmap((void *)address_2,stats_2.st_size);
  }
  if (fd_source != -1)
  {
    close(fd_source);
  }
  if (fd_dest != -1)
  {
    close(fd_dest);
  }
  return 0;
}
