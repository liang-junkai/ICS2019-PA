#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  //TODO();
  rtl_and(&s0,&id_dest->val,&id_src->val);
  if(s0==0){
    reg_f(ZF)=1;
  }
  else reg_f(ZF)=0;
  reg_f(CF)=0;
  reg_f(OF)=0;
  print_asm_template2(test);
}

make_EHelper(and) {
  //TODO();
  rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
  assert(id_dest->type==OP_TYPE_REG);
  rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  //TODO();
  rtl_xor(&id_dest->val,&id_dest->val,&id_src->val);
  if(id_dest->type==OP_TYPE_REG){
	rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
}
  print_asm_template2(xor);
}

make_EHelper(or) {
  //TODO();
  rtl_or(&s0,&id_dest->val,&id_src->val);
  id_dest->val=s0;
  reg_f(CF)=0;
  reg_f(OF)=0;
  rtl_update_ZFSF(&s0,id_dest->width);
  operand_write(id_dest,&id_dest->val);
  print_asm_template2(or);
}

make_EHelper(sar) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sar(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  //TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
