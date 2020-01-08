#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
   rtl_push(&cpu.EFLAGS.value);
   cpu.EFLAGS.IF=0;
   rtl_push(&cpu.cs);
   rtl_push(&ret_addr);
   assert(NO<=cpu.idtr.limit);
   vaddr_t gate_addr=cpu.idtr.base+8*NO;
   uint32_t low,high;
   low=vaddr_read(gate_addr,4)&0x0000ffff;
   high=vaddr_read(gate_addr+4,4)&0xffff0000;
   rtl_j(low|high);
}
#define IRQ_TIMER 32
bool isa_query_intr(void) {
  if ((cpu.EFLAGS.IF==1)&&cpu.INTR) {
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }
  return false;
}
