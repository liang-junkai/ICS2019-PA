#include "common.h"
#include "syscall.h"
#include"proc.h"
//#include<unistd.h>
void sys_yield(_Context *c){
  _yield();
  c->GPRx=0;
}
void sys_exit(_Context *c){
  int temp=c->GPR2;
  //printf("gp2: %d\n",c->GPR2);
  _halt(temp);
}
void sys_write(_Context *c,uintptr_t fd,uintptr_t buf,uintptr_t len){
  if(fd==1||fd==2){
    char *temp=(char*)buf;
    for(int i=0;i<len;i++){
      _putc(*temp++);
    }
    c->GPRx=len;
    return;
  }
  c->GPRx=-1;
}
void naive_uload(PCB *pcb, const char *filename);
void sys_execve(_Context *c){
  naive_uload(NULL,(char *)c->GPR2);
}
_Context* __am_irq_handle(_Context *c);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  //printf("a[0]: %d a[1]: %d a[2]: 0x%x a[3]: %d\n",a[0],a[1],a[2],a[3]);
  switch (a[0]) {
    case SYS_exit: sys_exit(c);break;
    case SYS_yield: sys_yield(c);break;
    case SYS_write: sys_write(c,a[1],a[2],a[3]);break;
    case SYS_execve: sys_execve(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  _Context *next=__am_irq_handle(c);
  return next;
}
