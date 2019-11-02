#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  //printf("src: %x\n",id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  //TODO();
  printf("dest:%d src:%d\n",id_dest->val,id_src->val);
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  //TODO();
  rtl_pop(&id_dest->val);
  operand_write(id_dest,&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  //TODO();
  reg_l(R_ESP)=reg_l(R_EBP);
  rtl_pop(&reg_l(R_EBP));
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&s0,R_AX,2);
    s1=s0>>15;
    if(s1==1){
      reg_w(R_DX)=0xffff;
    }
    if(s1==0){
      reg_w(R_DX)=0;
    }
    //TODO();
  }
  else {
    rtl_lr(&s0,R_EAX,4);
    s1=s0>>31;
    if(s1==1){
      reg_l(R_EDX)=0xffffffff;
    }
    if(s1==0){
      reg_l(R_EDX)=0;
    }
    //TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    rtl_lr(&t0,R_AX,1);
    rtl_sext(&t0,&t0,1);
    rtl_sr(R_AX,&t0,1);
    //TODO();
  }
  else {
    rtl_lr(&t0,R_AX,2);
    rtl_sext(&t0,&t0,2);
    rtl_sr(R_EAX,&t0,4);
    //TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
