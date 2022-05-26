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
  if (argc != 2)
  {
    fprintf(stderr, "(wrong usage )Please enter file name only");
    return 0;
  }
  const char *address = NULL;
  struct stat stats;
  int fd;
  fd = open(argv[1], O_RDONLY);
  if (fstat(fd, &stats) != 0)
  {
    perror("error file size");
  }
  address = mmap(NULL, stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (address == MAP_FAILED)
  {
    perror("address map failed");
  }
  
  if(address[0]!= 0x7f || address[1]!= 0x45 || address[2]!= 0x4c){
    fprintf(stderr,"ERROR: NOT ELF TYPE  \n");
    return 0;
  }
  Elf32_Ehdr * elfh=(Elf32_Ehdr * )address;
  int sc_id= elfh->e_ident[6];
  char * scheme=NULL;
  if(sc_id==0)
    scheme="Unknown data format";
  if(sc_id==1)
    scheme="Two's complement, little-endian";
  if(sc_id==2)
    scheme="Two's complement, big-endian";

  fprintf(stderr," ASCII ELF Magic : %x %x %x\n",address[0],address[1],address[2]);
  fprintf(stderr," Data Encoding Scheme : %s \n",scheme);
  fprintf(stderr," Entry point : 0x%x \n",elfh->e_entry);
  fprintf(stderr," Section header table file offset : %d (bytes into file) \n",elfh->e_shoff);
  fprintf(stderr," Number of section headers : %d \n",elfh->e_shnum);
  fprintf(stderr," Section header string table index : %d \n",elfh->e_shstrndx);

  if (address != MAP_FAILED)
  {
    munmap((void *)address, stats.st_size);
  }
  if (fd != -1){
    close(fd);
  }
  return 0;
}