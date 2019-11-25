#include "common.h"
#include "syscall.h"
void sys_yield(_Context *c){
  _yield();
  c->GPRx=0;
}
void sys_exit(_Context *c){
  int temp=c->GPR2;
  //printf("gp2: %d\n",c->GPR2);
  _halt(temp);
}
void sys_write(uintptr_t fd,uintptr_t buf,uintptr_t len){
  //c->GPRx=fs_wri
}
_Context* __am_irq_handle(_Context *c);
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  printf("a[0]: %d\n",a[0]);
  switch (a[0]) {
    case SYS_exit: sys_exit(c);break;
    case SYS_yield: sys_yield(c);break;
    case SYS_write: 
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  _Context *next=__am_irq_handle(c);
  return NULL;
  return next;
}
