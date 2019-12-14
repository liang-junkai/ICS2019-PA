#include <am.h>
#include <x86.h>
//#include<stdio.h>
#include<klib.h>
//#include"proc.h"
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void __am_irq0();
void __am_vecsys();
void __am_vectrap();
void __am_vecnull();
//extern int printf(const char *fmt,...);
_Context* __am_irq_handle(_Context *c) {
  _Context *next = c;
  //printf("c->eip: %x\n",c->eip);
  if (user_handler) {
    _Event ev = {0};
    switch (c->irq) {
      case 0x80:ev.event=_EVENT_SYSCALL;break;
      case 0x81:ev.event=_EVENT_YIELD;break;
      default: ev.event = _EVENT_ERROR; break;
    }
    //printf("next:\n");
    next = user_handler(ev, c);
    //printf("handle: eip:0x%d\n",next->eip);
    if (next == NULL) {
      next = c;
    }
  }
  //printf("pcb: 0x%x\n",pcb[0].cp->eip);
  //printf("handle: eip:0x%d\n",next->eip);
  return next;
}

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  static GateDesc idt[NR_IRQ];

  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecnull, DPL_KERN);
  }

  // ----------------------- interrupts ----------------------------
  idt[32]   = GATE(STS_IG32, KSEL(SEG_KCODE), __am_irq0,   DPL_KERN);
  // ---------------------- system call ----------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vecsys, DPL_USER);
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), __am_vectrap, DPL_KERN);

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;
  
  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  _Context *c=(_Context*)stack.end-1;
  c->cs=8;
  c->eip=(uintptr_t)entry;
  printf("eip: 0x%x\n",c->eip);
  return c;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
