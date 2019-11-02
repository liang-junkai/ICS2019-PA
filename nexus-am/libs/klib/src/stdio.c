#include "klib.h"
#include <stdarg.h>
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int val;
  va_start(args,fmt);
  val=vsprintf(out,fmt,args);
  va_end(args);
  return val;
}
int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}
int printf(const char *fmt, ...) {
  va_list args;
  char buf1[1024];
  buf1[0]='\0';
  va_start(args,fmt);
  int val=sprintf(buf1,fmt,args);
  va_end(args); 
  for(int i=0;buf1[i]!='\0';i++){
    _putc(buf1[i]);
  }
  return val;
}
//static char* digits="0123456789abcdef";
//static char* upper_digits="0123456789ABCDEF";
/*static char*number(char *str,long num,int base){
  char temp[100];
  char *dig=digits;
  int i=0;
  while(num!=0){
    temp[i++]=dig[((unsigned long)num)%(unsigned)base];
    num=((unsigned long)num)/(unsigned)base;
  }
  while(i-->0)*str++=temp[i];
  return str;
}*/
void print(char *str,int n){
  int i;
  for(i=0;i<n;i++){
    _putc(*str++);
  }
  _putc('\n');
}
int vsprintf(char *out, const char *fmt, va_list ap) {
  unsigned long n;
  char *s;
  char *str=out;
  int qualifier=-1;
  while(*fmt!='\0'){
      if(*fmt!='%'){
          *str=*fmt;
          str++;
          fmt++;
          continue;
      }
      fmt++;
      if(*fmt=='l'){
          qualifier=*fmt;
          fmt++;
      }
      switch(*fmt){
          case 's':
            s=va_arg(ap,char *);
            int len=strlen(s);
            for(int i=0;i<len;i++){
                *str=*s;
                str++;
                s++;

            }
            break;
          case 'd':
            if(qualifier=='l'){
                n=va_arg(ap,unsigned long);
                }
                else{
                    n=va_arg(ap,int);
                }
                char num[100];
                int s1=0;
                while(n>0){
                    int t=n%10;
                    n=n/10;
                    num[s1]=t+48;
                    s1++;
                }
                s1--;
                while(s1>=0){
                    *str=num[s1];
                    str++;
                    s1--;
                }
                break;
            case 'c':
                *str=(unsigned char)va_arg(ap,int);
                str++;
                break;
            default:
                break;
      }
        fmt++;
  }
    *str='\0';
    return str-out;
}




#endif
