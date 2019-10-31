#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if(ref_r->eax!=reg_l(R_EAX)||ref_r->ebx!=reg_l(R_EBX)||ref_r->ecx!=reg_l(R_ECX)
  ||ref_r->edx!=reg_l(R_EDX)||ref_r->ebp!=reg_l(R_EBP)||ref_r->esi!=reg_l(R_ESI)||
  ref_r->edi!=reg_l(R_EDI)||ref_r->pc!=cpu.pc){
    if(ref_r->eax!=cpu.ecx)printf("->");
    printf("r.eax: %-8x  cpu.eax: %-8x\n",ref_r->eax,cpu.eax);
    if(ref_r->ecx!=cpu.ecx)printf("->");
    printf("r.ecx: %-8x  cpu.ecx: %-8x\n",ref_r->ecx,cpu.ecx);
    if(ref_r->edx!=cpu.edx)printf("->");
    printf("r.edx: %-8x  cpu.edx: %-8x\n",ref_r->edx,cpu.edx);
    if(ref_r->ebx!=cpu.ebx)printf("->");
    printf("r.ebx: %-8x  cpu.ebx: %-8x\n",ref_r->ebx,cpu.ebx);
    if(ref_r->esp!=cpu.esp)printf("->");
    printf("r.esp: %-8x  cpu.esp: %-8x\n",ref_r->esp,cpu.esp);
    if(ref_r->ebp!=cpu.ebp)printf("->");
    printf("r.ebp: %-8x  cpu.esp: %-8x\n",ref_r->ebp,cpu.ebp);
    if(ref_r->esi!=cpu.esi)printf("->");
    printf("r.esi: %-8x  cpu.esi: %-8x\n",ref_r->esi,cpu.esi);
    if(ref_r->edi!=cpu.edi)printf("->");
    printf("r.edi: %-8x  cpu.edi: %-8x\n",ref_r->edi,cpu.edi);
    if(ref_r->pc!=cpu.pc)printf("->");
    printf("r.pc:  %-8x  cpu.pc:  %-8x\n",ref_r->pc,cpu.pc);
    return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
