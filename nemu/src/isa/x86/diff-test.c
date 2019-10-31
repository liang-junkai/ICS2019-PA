#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if(ref_r->eax!=reg_l(R_EAX)||ref_r->ebx!=reg_l(R_EBX)||ref_r->ecx!=reg_l(R_ECX)
  ||ref_r->edx!=reg_l(R_EDX)||ref_r->ebp!=reg_l(R_EBP)||ref_r->esi!=reg_l(R_ESI)||
  ref_r->edi!=reg_l(R_EDI)||ref_r->pc!=cpu.pc){
    printf("r.eax: %-8x  cpu.eax: %-8x\n",ref_r->eax,cpu.eax);
    printf("r.ecx: %-8x  cpu.ecx: %-8x\n",ref_r->ecx,cpu.ecx);
    printf("r.edx: %-8x  cpu.edx: %-8x\n",ref_r->edx,cpu.edx);
    printf("r.ebx: %-8x  cpu.ebx: %-8x\n",ref_r->ebx,cpu.ebx);
    printf("r.esp: %-8x  cpu.esp: %-8x\n",ref_r->esp,cpu.esp);
    printf("r.ebp: %-8x  cpu.esp: %-8x\n",ref_r->ebp,cpu.ebp);
    printf("r.esi: %-8x  cpu.esi: %-8x\n",ref_r->esi,cpu.esi);
    printf("r.edi: %-8x  cpu.edi: %-8x\n",ref_r->edi,cpu.edi);
    printf("r.pc:  %-8x  cpu.pc:  %-8x\n",ref_r->pc,cpu.pc);
    return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
