#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


int printf(const char *fmt, ...) {
  return 0;
}
static char* digits="0123456789abcdef";
//static char* upper_digits="0123456789ABCDEF";
static char*number(char *str,long num,int base){
  char temp[100];
  char *dig=digits;
  int i=0;
  while(num!=0){
    temp[i++]=dig[((unsigned long)num)%(unsigned)base];
    num=((unsigned long)num)/(unsigned)base;
  }
  while(i-->0)*str++=temp[i];
  return str;
}
int vsprintf(char *out, const char *fmt, va_list args) {
  unsigned long num;
  int len;
  char *str;
  char *s;
  int i;
  for(str=out;*fmt;fmt++){
    if(*fmt!='%'){
      *str=*fmt;
      str++;
    }
    fmt++;
    switch(*fmt){
      case 'd':
        num=va_arg(args,int);
        str=number(str,num,10);
        continue;
      case 's':
        s = va_arg(args, char *);
        if (!s) s = "<NULL>";
        len = strlen(s);
        for (i = 0; i < len; ++i) *str++ = *s++;
        continue;
    }

  }
  *str='\0';
  return str-out;
}

int sprintf(char *out, const char *fmt, ...) {
  /*va_list args;
  int val;
  va_start(args,fmt);
  val=vsprintf(out,fmt,args);
  va_end(args);
  return val;*/
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
