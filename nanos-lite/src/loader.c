#include "proc.h"
#include <elf.h>
//#include<stdio.h>
#include"fs.h"
#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
#define DEFAULT_ENTRY 0x3000000
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();
static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  //FILE *fp=fopen(filename,"rb");
  //Elf_Ehdr *elf_e=NULL;
  //int result=fread(elf_e,sizeof(Elf_Ehdr),1,fp);
  //result+=1;
  //lseek(fp,0,SEEK_SET);
  //Elf_Phdr elf_p;
  //lseek(fp,elf_e.e_shoff,SEEK_SET);
  //fread(elf_p,sizeof(struct Elf_Phdr),1,fp);
  ramdisk_read((void *)DEFAULT_ENTRY,0,get_ramdisk_size());
  //printf("ramdisksize: %x\n",get_ramdisk_size());
  //printf("elf.entry: %d\n",elf_e->e_entry);
  //return (uintptr_t)(DEFAULT_ENTRY+0x10fc);
  return (DEFAULT_ENTRY+0x10e0);
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
