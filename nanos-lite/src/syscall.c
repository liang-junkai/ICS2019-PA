#include "common.h"
#include "syscall.h"
#include"proc.h"
#include<assert.h>
//#include<unistd.h>
void naive_uload(PCB *pcb, const char *filename);
void sys_yield(_Context *c){
  _yield();
  c->GPRx=0;
}
void sys_exit(_Context *c){
  //int temp=c->GPR2;
  //printf("gp2: %d\n",c->GPR2);
  //_halt(temp);
  _halt(c->GPR2);
  naive_uload(NULL,"/bin/init");
}
size_t fs_read(int fd,void *buf,size_t len);
size_t fs_write(int fd,const void *buf,size_t len);
void sys_write(_Context *c,uintptr_t fd,uintptr_t buf,uintptr_t len){
  //printf("write\n");
  /*if(fd==1||fd==2){
    char *temp=(char*)buf;
    for(int i=0;i<len;i++){
      _putc(*temp++);
    }
    c->GPRx=len;
    return;
  }*/
  if(len==0)printf("write: 00000000\n");
  c->GPRx=fs_write(fd,(void*)buf,len);
}

void sys_execve(_Context *c){
  //printf("%s\n",(char*)c->GPR2);
  //panic("ljk never fail\n");
  naive_uload(NULL,(char*)c->GPR2);
}
int mm_brk(uintptr_t brk, intptr_t increment);
void sys_brk(_Context*c){
  c->GPRx=mm_brk(c->GPR2,c->GPR3);
}
size_t fs_open(const char* pathname,int flags,int mode);
void sys_open(_Context*c){
  c->GPRx=fs_open((char*)c->GPR2,c->GPR3,c->GPR4);
}
size_t fs_close(int fd);
void sys_close(_Context*c){
  c->GPRx=fs_close(c->GPR2);
}
size_t fs_lseek(int fd,size_t offset,int whence);
void sys_lseek(_Context*c){
  c->GPRx=fs_lseek(c->GPR2,c->GPR3,c->GPR4);
}
size_t fs_read(int fd,void *buf,size_t len);
void sys_read(_Context*c){
  if(c->GPR4==0)printf("ljkljlkljkl\n");
  c->GPRx=fs_read(c->GPR2,(void*)c->GPR3,c->GPR4);
}
_Context* __am_irq_handle(_Context *c);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  //printf("GPR1: %x GPR2: %x GPR3: 0x%x GPR4: %x GPRx: %x\n",c->GPR1,c->GPR2,c->GPR3,c->GPR4,c->GPRx);
  switch (a[0]) {
    case SYS_exit: sys_exit(c);break;
    case SYS_yield: sys_yield(c);break;
    case SYS_write: sys_write(c,c->GPR2,c->GPR3,c->GPR4);break;
    case SYS_open: sys_open(c);break;
    case SYS_close: sys_close(c);break;
    case SYS_lseek: sys_lseek(c);break;
    case SYS_brk:sys_brk(c);break;
    case SYS_read: sys_read(c);break;
    case SYS_execve: sys_execve(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  //_Context *next=__am_irq_handle(c);
  return NULL;
}
