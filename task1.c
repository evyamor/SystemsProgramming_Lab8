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
/*typedef Elf32_Shdr;
typedef Elf32_Ehdr;
*/

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
/*
int elf_get_symval(Elf32_Ehdr *hdr, int table, uint idx)
{
  if (table == SHN_UNDEF || idx == SHN_UNDEF)
    return 0;
  Elf32_Shdr *symtab = elf_section(hdr, table);
  uint32_t symtab_entries = symtab->sh_size / symtab->sh_entsize;
  if (idx >= symtab_entries)
  {
    perror("Symbol idx out of bounds");
    return -1;
  }
  int symaddr = (int)hdr + symtab->sh_offset;
  Elf32_Sym *symbol = &((Elf32_Sym *)symaddr[idx]);
  if (symbol->st_shndx == SHN_UNDEF)
  {
    Elf32_Shdr *strtab = elf_section(hdr, symtab->sh_link);
    char *name = (char *)hdr + strtab->sh_offset + symbol->st_name;

    extern void *elf_lookup_symbol(char *name);
    void *target = elf_lookup_symbol(name);

    if (target == NULL)
    {
      if (ELF32_ST_BIND(symbol->st_info) & STB_WEAK)
        return 0;
      return -1;
    }
    else
    {
      return (int)target;
    }
  }
  else if (symbol->st_shndx == SHN_ABS)
  {
    return symbol->st_value;
  }
  else
  {
    Elf32_Shdr *target = elf_section(hdr, symbol->st_shndx);
    return (int)hdr + symbol->st_value + target->sh_offset;
  }
}
}
*/
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
    int symaddr = (int)elfh + section->sh_offset;
    Elf32_Sym *symbol = (Elf32_Sym *)symaddr;
    fprintf(stderr, "%d %s 0x%x 0x%x %d\n", i, elf_lookup_string(elfh,(int)section->sh_name), section->sh_addr, section->sh_offset, section->sh_size);
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
