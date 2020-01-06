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
size_t fs_read(int fd,void *buf,size_t len);
size_t fs_openset(int fd);
size_t fs_diskset(int fd);
size_t fs_open(const char* pathname,int flags,int mode);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_read2(int fd,void *buf,size_t len);
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
  //ramdisk_read((void *)DEFAULT_ENTRY,0,get_ramdisk_size());
  //printf("ramdisksize: %x\n",get_ramdisk_size());
  //printf("elf.entry: %d\n",elf_e->e_entry);
  //return (uintptr_t)(DEFAULT_ENTRY+0x10fc);
  Elf_Ehdr ehdr;
  size_t fd=fs_open(filename,0,0);
  size_t size=fs_read(fd,&ehdr,sizeof(Elf_Ehdr));
  size++;
  int n=ehdr.e_phnum;
  Elf_Phdr phdr;
  for(int i=0;i<n;i++){
    ramdisk_read(&phdr,ehdr.e_phoff+sizeof(Elf_Phdr)*i+fs_diskset(fd),sizeof(Elf_Phdr));
    if(phdr.p_type==PT_LOAD){
      ramdisk_read((void*)phdr.p_vaddr,phdr.p_offset+fs_diskset(fd),phdr.p_memsz);
      memset((void*)(phdr.p_vaddr+phdr.p_filesz),0,phdr.p_memsz-phdr.p_filesz);
    }
  }
  return ehdr.e_entry;
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
