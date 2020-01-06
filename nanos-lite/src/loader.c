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
# define vaddr_t uint32_t
# define paddr_t uint32_t
#endif
#define DEFAULT_ENTRY 0x3000000
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();
size_t fs_read(int fd,void *buf,size_t len);
size_t fs_openset(int fd);
size_t fs_diskset(int fd);
size_t fs_close(int fd);
size_t fs_lseek(int fd, size_t offset, int whence);
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
  /*Elf_Ehdr ehdr;
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
  return ehdr.e_entry;*/
  int fd=fs_open(filename,0,0);
  Elf_Ehdr header;
  fs_read(fd,(void*)&header,sizeof(header));
  Elf_Phdr phdr[header.e_phnum];
  char buf[4096];
  fs_lseek(fd, header.e_phoff, 0);
  fs_read(fd, (void*) &phdr, header.e_phnum*header.e_phentsize);
  for (int index=0; index<header.e_phnum; index++) {
    if (phdr[index].p_type != PT_LOAD) 
      continue;
    fs_lseek(fd, phdr[index].p_offset, 0);
    int left = phdr[index].p_filesz;
    int last_read = 0;
    //void *va=DEFAULT_ENTRY;
    //void *pa=DEFAULT_ENTRY;
    vaddr_t va=0;
    paddr_t pa=0;
    while (left>0) {
      if (left>=4096){
        fs_read(fd, (void*) buf, 4096);
        va = (phdr[index].p_vaddr+phdr[index].p_filesz-left);
        pa = (paddr_t)new_page(1);
        // Log("set map va=%d, pa=%d\n", va, pa);
        _map(&pcb->as, (void*)va, (void*)pa, 1);
        memcpy((void*)pa, buf, 4096);
        left-=4096;
        last_read = 4096;
      }
      else {
        fs_read(fd, (void*) buf, left);
        va = (phdr[index].p_vaddr+phdr[index].p_filesz-left);
        pa = (paddr_t)new_page(1);
        // Log("set map va=%d, pa=%d\n", va, pa);
        _map(&pcb->as, (void*)va, (void*)pa, 1);
        memcpy((void*)pa, buf, left);
        last_read = left;
        left-=left;
      }
    }
    left = phdr[index].p_memsz-phdr[index].p_filesz;
    if (last_read!=4096) {
      va = (phdr[index].p_vaddr+phdr[index].p_filesz);
      int res = 4096-last_read;
      if (res>=left) {
        memset((void*)(pa+last_read), 0, left);
        continue;
      }
      else {
        memset((void*)(pa+last_read), 0, res);
        left-=res;
      }
      while (left>0) {
        va = (phdr[index].p_vaddr + phdr[index].p_memsz-left);
        pa = (paddr_t)new_page(1);
        _map(&pcb->as, (void*)va, (void*)pa, 1);
        if (left>=4096) {
          memset((void*)pa, 0, 4096);
          left-=4096;
        }
        else {
          memset((void*)pa, 0, left);
          left-=left;
        }
      } 
    }
  }
  fs_close(fd);
  return header.e_entry;
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
