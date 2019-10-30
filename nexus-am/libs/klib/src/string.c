#include "klib.h"
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  return 0;
}

char *strcpy(char* dst,const char* src) {
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  return NULL;
}

int strcmp(const char* s1, const char* s2) {
  while(*s1==*s2){
    _putc(*s1);
    _putc(' ');
    _putc(*s2);
    _putc('\n');
    if(*s1=='\0'||*s2=='\0')break;
    s1++;
    s2++;
  }
  if(*s1-*s2<0){
    _putc('l');
    return -1;
    }
    _putc('k');
  return (int)(*s1-*s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  for(int i=0;i<n;i++){
    if(*(char*)s1=='\0'||*(char*)s2=='\0')break;
    if(*(char*)s1!=*(char*)s2)break;
    s1++;
    s2++;
  }
  if(*(char *)s1-*(char *)s2<0)return -1;
  return *(char *)s1-*(char *)s2;
  return memcmp(s1,s2,n);
}

#endif
