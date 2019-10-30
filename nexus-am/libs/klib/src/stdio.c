#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  /*char *str;
  int field_width;
  for(str=out;*fmt;fmt++){
    unsigned long num;
    int32_t base=10;
    int32_t flags=0;
    int32_t qualifier=-1;
    int32_t precision=-1;
    bool bFmt=true;
    if(*fmt!='%'){
      *str++=*fmt;
      continue;
    }
  }
  bFmt=true;
  while(bFmt){
    fmt++;
    switch(*fmt){
      case '-':flags|=LEFT;break;
      case '+':flags|=PLUS;break;
      case ' ':flags|=SPACE;break;
      case '#':flags|=SPECIAL;break;
      case '0':flags|=ZEROPAD;break;
      default :bFmt=false;
    }
    field_width=-1;
    if(is_digit(*fmt)){
      field_width=skip_atoi(&fmt);
    }
    else if('*'==*fmt){
      fmt++;
      field_width=va_arg(args,int);
      if(field_width<0){
        field_width=-field_width;
        flags|=LEFT;
      }
    }
    precision=-1;
    if('.'==*fmt){
      ++fmt;
      if(is_digit(*fmt)){
        precision=skip_atoi(&fmt);
      }
      else if('*'==*fmt){
        ++fmt;
        precision=va_arg(args,int);
      }
      if(precision<0)precision=0;
    }
    qualifier=-1;
    if('h'==*fmt||'l'==*fmt||'L'==*fmt){
      qualifier=*fmt;
      fmt++;
    }
    base=10;
    switch (*fmt){
      case 'c':{
        if (!(flags & LEFT)) while (--field_width > 0) *str++ = ' ';
        *str++ = (unsigned char) va_arg(args, int);
        while (--field_width > 0) *str++ = ' ';
        continue;
        }
      case 's':{
        int len;
        char * s = va_arg(args, char *);
        if (!s) s = "<NULL>";
        len = strnlen(s, precision);
        if (!(flags & LEFT)) while (len < field_width--) *str++ = ' ';
        for (int i = 0; i < len; ++i) *str++ = *s++;
        while (len < field_width--) *str++ = ' ';
        continue;
        }
      case 'p':{
        if (-1 == field_width){
          field_width = 2 * sizeof(void *);
          flags |= ZEROPAD;
          }
        str = number(str, (unsigned long) va_arg(args, void *), 16, field_width, precision, flags);
        continue;
        }
      case 'n':{
        if ('l' == qualifier){
          long *ip = va_arg(args, long *);
          *ip = (str - buf);
          }
        else{
          int *ip = va_arg(args, int *);
          *ip = (str - buf);
          }
          continue;
          }
      case 'A':{
        flags |= LARGE; 
        }
      case 'a':{
        if ('l' == qualifier){
          str = eaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
          }
        else{
          str = iaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
          }
        continue;
        }
                
      case 'o':{
        base = 8;
        break;
        }
      case 'X':{
        flags |= LARGE; 
        }
      case 'x':{
        base = 16;
        break;
        }
      case 'd':
      case 'i':{
        flags |= SIGN; 
        }
      case 'u':{
        break;
        }
#ifndef NOFLOAT
      case 'E':
      case 'G':
      case 'e':
      case 'f':
      case 'g':
      {
        str = flt(str, va_arg(args, double), field_width, precision, *fmt, flags | SIGN);
        continue;
        }
#endif
      default:
      {
      if (*fmt != '%') *str++ = '%';
      if (*fmt)
      {
      *str++ = *fmt;
      }
      else
      {
      --fmt;
      }
      continue;
              }
  }
 
  if (qualifier == 'l')
        {
            num = va_arg(args, unsigned long);
        }
  else if (qualifier == 'h')
        {
  if (flags & SIGN)
                num = va_arg(args, short);
  else
                num = va_arg(args, unsigned short);
        }
  else if (flags & SIGN)
        {
            num = va_arg(args, int);
        }
  else
        {
            num = va_arg(args, unsigned long);
        }
        
  str = number(str, num, base, field_width, precision, flags);
    } 
 
  *str = '/0';
  return str - buf;*/
  return 0;
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
