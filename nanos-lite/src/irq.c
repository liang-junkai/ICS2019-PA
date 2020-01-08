#include "common.h"
_Context* do_syscall(_Context *c);
_Context* schedule(_Context *prev);
static _Context* do_event(_Event e, _Context* c) {
  //printf("do_event\n");
  switch (e.event) {
    case _EVENT_SYSCALL: do_syscall(c);break;
    case _EVENT_YIELD: {
      printf("you have reached _EVENT_YIELD\n");
      _Context *c2=schedule(c);
      //printf("c2: 0x%x\n",c2->eip);
      return c2;
      break;
    }
    case _EVENT_IRQ_TIMER:{
      //Log("Timer interrupt");
      _yield();
      break;
    }
    default: panic("Unhandled event ID = %d", e.event);
  }
  //printf("do_event c:%d\n",c->GPRx);
  //printf("you have exit do_event\n");
  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
