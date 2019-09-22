#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
	int i;
	for(i=R_EAX;i<=R_EDI;i++){
		switch(i){
			case R_EAX:printf("eax :\t");break;
			case R_ECX:printf("ecx :\t");break;
			case R_EDX:printf("edx :\t");break;
			case R_EBX:printf("ebx :\t");break;
			case R_ESP:printf("esp :\t");break;
			case R_EBP:printf("ebp :\t");break;
			case R_ESI:printf("esi :\t");break;
			case R_EDI:printf("edi :\t");break;
}
		printf("0x%-15x",reg_l(i));
		printf("%-15d\n",reg_l(i));
}
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
 int i;
 *success=false;
 for(i=R_EAX;i<=R_EDI;i++){
	if(strcmp(s,regsl[i])==0){
		*success=true;
		return reg_l(i);
	}
 }
 for(i=R_AX;i<=R_DI;i++){
	if(strcmp(s,regsw[i])==0){
		*success=true;
		return reg_w(i);
	}
 }
 for(i=R_AL;i<=R_DI;i++){
	if(strcmp(s,regsb[i])==0){
		*success=true;
		return reg_b(i);
	}
 }
 return 0;
}
