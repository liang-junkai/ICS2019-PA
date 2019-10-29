#include "rtl/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:{
      if(reg_f(OF)==1)*dest=1;
      else *dest=0;
      break;
    }
    case CC_B:{
      if(reg_f(CF)==1&&reg_f(ZF)==0){
        *dest=1;
      }
      else *dest=0;
      break;
    }
    case CC_E:{
      if(reg_f(ZF)==1){
        *dest=1;
      }
      else{
        *dest=0;
      }
      break;
    }
    case CC_BE:{
      if(reg_f(CF)==1||reg_f(ZF)==1){
        *dest=1;
      }
      else *dest=0;
      break;
    }
    case CC_S:{
      if(reg_f(SF)==1)*dest=1;
      else *dest=0;
      break;
    }
    case CC_L:{
      if(reg_f(SF)!=reg_f(OF)&&reg_f(ZF)==0)*dest=1;
      else *dest=0;
      break;
    }
    case CC_LE:{
      if(reg_f(SF)!=reg_f(OF)||reg_f(ZF)==1)*dest=1;
      else *dest=0;
      break;
    }
      //TODO();
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}
