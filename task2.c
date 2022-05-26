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


Elf32_Shdr *elf_sheader(Elf32_Ehdr *hdr)
{
  return (Elf32_Shdr *)((int)hdr + hdr->e_shoff);
}
Elf32_Shdr *elf_section(Elf32_Ehdr *hdr, int index)
{
  return &elf_sheader(hdr)[index];
}
char *elf_str_table(Elf32_Ehdr *hdr)
{
  if (hdr->e_shstrndx == SHN_UNDEF)
    return NULL;
  return (char *)hdr + (elf_section(hdr, hdr->e_shstrndx)->sh_offset);
}
char *elf_lookup_string(Elf32_Ehdr *hdr, int offset)
{
  char *strtab = elf_str_table(hdr);
  if (strtab == NULL)
    return NULL;
  return strtab + offset;
}


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

  if (address[0] != 0x7f || address[1] != 0x45 || address[2] != 0x4c)
  {
    fprintf(stderr, "ERROR: NOT ELF TYPE  \n");
    return 0;
  }
  Elf32_Ehdr *elfh = (Elf32_Ehdr *)address;
  fprintf(stderr, "Layout table \n[index]|section_name|section_adress|section_offset|section_size \n");
  Elf32_Shdr * shdr=elf_sheader(elfh);
  int offs=0;
  for (int i = 0; i < elfh->e_shnum; i++)
  {
    Elf32_Shdr *section=&shdr[i];
    fprintf(stderr, "%d %s 0x%x 0x%x %d\n", i, elf_lookup_string(elfh,(int)section->sh_name), section->sh_addr, section->sh_offset, section->sh_size);
  }
  fprintf(stderr, "\n\n Layout table 2 \n[index]|value|section_index|section_name|symbol_name \n");
  int glo_idx=0; 
    Elf32_Shdr *section = (Elf32_Shdr*)(address+elfh->e_shoff);
    char* section_names = (char*)(address+section[elfh->e_shstrndx].sh_offset);
    for(int i=0;i<elfh->e_shnum;i++){
      if(section[i].sh_type == SHT_SYMTAB){
        fprintf(stderr,"\nTable .symtab :\n");
        Elf32_Sym * symtab=(Elf32_Sym *)(address+section[i].sh_offset);
        int symbol_n =section[i].sh_size/section[i].sh_entsize;
        char* symbol_names= (char *)(address+section[section[i].sh_link].sh_offset);
        for(int j=0;j<symbol_n;j++){
          fprintf(stderr, "%d 0x%x %d %s %s \n", glo_idx,symtab[j].st_value,symtab[j].st_shndx,
             section_names+section[i].sh_name, symbol_names +symtab[j].st_name);
             glo_idx++;
        }
      }
      
      if(section[i].sh_type == SHT_DYNSYM){
        fprintf(stderr,"\nTable .dynsm :\n");
        Elf32_Sym * dynsm=(Elf32_Sym *)(address+section[i].sh_offset);
        int symbol_n =section[i].sh_size/section[i].sh_entsize;
        char* symbol_names= (char *)(address+section[section[i].sh_link].sh_offset);
        for(int j=0;j<symbol_n;j++){
          fprintf(stderr, "%d 0x%x %d %s %s \n", glo_idx,dynsm[j].st_value,dynsm[j].st_shndx,
             section_names+section[i].sh_name, symbol_names +dynsm[j].st_name);
             glo_idx++;
        }
      }
    }
      if (address != MAP_FAILED)
  {
    munmap((void *)address, stats.st_size);
  }
  if (fd != -1)
  {
    close(fd);
  }
 
  return 0;
}
