#include "klib.h"
#include <stdarg.h>
#include<stdio.h>
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
#define ZEROPAD    1       /* pad with zero */
 #define SIGN   2       /* unsigned/signed long */
 #define PLUS   4       /* show plus */
 #define SPACE  8       /* space if plus */
 #define LEFT   16      /* left justified */
 #define SPECIAL    32      /* 0x */
 #define LARGE  64      /* use 'ABCDEF' instead of 'abcdef' */

 int _div(long* n,unsigned base)
 {
     int __res; 
         __res = ((unsigned long) *n) % (unsigned) base; 
         *n = ((unsigned long) *n) / (unsigned) base; 
         return __res;
 }

#define do_div(n,base) _div(&n,base)/*({ \
    int __res; \
    __res = ((unsigned long) n) % (unsigned) base; \
    n = ((unsigned long) n) / (unsigned) base; \
    __res; })*/




 static inline int isdigit(int ch)
 {
    return (ch >= '0') && (ch <= '9');
 }



 static int skip_atoi(const char **s)
 {
    int i = 0;

    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
 }



 static char *Test_number(char *str, long num, int base, int size, int precision,
    int type)
 {
    char c, sign, tmp[66];
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i;

    if (type & LARGE)
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL) {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
    {
        tmp[i++] = '0';
    }
    else
    {
        while (num != 0)
        {
            tmp[i++] = digits[do_div(num, base)];
        }
    }

    if (i > precision)
        precision = i;
    size -= precision;
    if (!(type & (ZEROPAD + LEFT)))
        while (size-- > 0)
            *str++ = ' ';
    if (sign)
        *str++ = sign;
    if (type & SPECIAL) {
        if (base == 8)
            *str++ = '0';
        else if (base == 16) {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (!(type & LEFT))
        while (size-- > 0)
            *str++ = c;
    while (i < precision--)
        *str++ = '0';
    while (i-- > 0)
        *str++ = tmp[i];
    while (size-- > 0)
        *str++ = ' ';
    return str;
 }




 int vsprintf(char *buf, const char *fmt, va_list args)
 {
    int len;
    unsigned long num;
    int i, base;
    char *str;
    const char *s;

    int flags;      /* flags to Test_number() */

    int field_width;    /* width of output field */
    int precision;      /* min. # of digits for integers; max
                   Test_number of chars for from string */
    int qualifier;      /* 'h', 'l', or 'L' for integer fields */

    for (str = buf; *fmt; ++fmt) {
        if (*fmt != '%') {
            *str++ = *fmt;
            continue;
        }

        /* process flags */
        flags = 0;
          repeat:
        ++fmt;      /* this also skips first '%' */
        switch (*fmt) {
        case '-':
            flags |= LEFT;
            goto repeat;
        case '+':
            flags |= PLUS;
            goto repeat;
        case ' ':
            flags |= SPACE;
            goto repeat;
        case '#':
            flags |= SPECIAL;
            goto repeat;
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }

        /* get field width */
        field_width = -1;
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (isdigit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
            qualifier = *fmt;
            ++fmt;
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT))
                while (--field_width > 0)
                    *str++ = ' ';
            *str++ = (unsigned char)va_arg(args, int);
            while (--field_width > 0)
                *str++ = ' ';
            continue;

        case 's':
            s = va_arg(args, char *);
            len = strnlen(s, precision);

            if (!(flags & LEFT))
                while (len < field_width--)
                    *str++ = ' ';
            for (i = 0; i < len; ++i)
                *str++ = *s++;
            while (len < field_width--)
                *str++ = ' ';
            continue;

        case 'p':
            if (field_width == -1) {
                field_width = 2 * sizeof(void *);
                flags |= ZEROPAD;
            }
            str = Test_number(str,
                     (unsigned long)va_arg(args, void *), 16,
                     field_width, precision, flags);
            continue;

        case 'n':
            if (qualifier == 'l') {
                long *ip = va_arg(args, long *);
                *ip = (str - buf);
            } else {
                int *ip = va_arg(args, int *);
                *ip = (str - buf);
            }
            continue;

        case '%':
            *str++ = '%';
            continue;

            /* integer Test_number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            *str++ = '%';
            if (*fmt)
                *str++ = *fmt;
            else
                --fmt;
            continue;
        }
        if (qualifier == 'l')
            num = va_arg(args, unsigned long);
        else if (qualifier == 'h') {
            num = (unsigned short)va_arg(args, int);
            if (flags & SIGN)
                num = (short)num;
        } else if (flags & SIGN)
            num = va_arg(args, int);
        else
            num = va_arg(args, unsigned int);
        str = Test_number(str, num, base, field_width, precision, flags);
    }
    *str = '\0';
    return str - buf;
 }
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
  for(int i=0;i<strlen(buf1);i++){
    _putc(buf1[i]);
  }
  return val;
}
/*static char* digits="0123456789abcdef";
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
void print(char *str,int n){
  int i;
  for(i=0;i<n;i++){
    _putc(*str++);
  }
  _putc('\n');
}
int vsprintf(char *out, const char *fmt, va_list args) {
  unsigned long num;
  int len;
  char *str;
  char *s;
  int i;
  //int nstr=0;
  for(str=out;*fmt!='\0';fmt++){
    if(*fmt!='%'){
      *str=*fmt;
      str++;
    }
    else{
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
      default:
        continue;
    }
  }
  }
  *str='\0';
  for(int j=0;j<strlen(out);j++){
    _putc(out[j]);
  }
  //_putc('\n');
 return str-out;
}


*/

#endif
