#include "common.h"
#include "syscall.h"
static inline uintptr_t sys_yield(_Context *c){
  _yield();
  printf("you have exit event_yield\n");
  c->GPRx=0;
  return 1;
}
_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case 1: sys_yield(c);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
