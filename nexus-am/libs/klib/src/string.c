#include "klib.h"
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  int len=0;
  while(s[len]!='\0'){
    len++;
  }
  return len;
}
size_t strnlen(const char* s,size_t count){
  const char *sc;

    for (sc = s; count-- && *sc != '\0'; ++sc);
    return sc - s;
}
char *strcpy(char* dst,const char* src) {
  int i=0;
  for(i=0;i<strlen(src);i++){
    dst[i]=src[i];
  }
  dst[i]='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  int len1=strlen(dst);
  int i=0;
  for(i=0;i<strlen(src);i++){
    dst[len1+i]=src[i];
  }
  dst[len1+i]='\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  for(int i=0;i<strlen(s1)&&i<strlen(s2);i++){
    if(s1[i]<s2[i])return -1;
    if(s1[i]>s2[i])return 1;
  }
  if(strlen(s1)<strlen(s2))return -1;
  if(strlen(s1)>strlen(s2))return 1;
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
   if (!n)  return 0;
 
   while (--n && *s1 && *s1 == *s2) s1++,s2++;
 
   return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
}

void* memset(void* v,int c,size_t n) {
  if(v==NULL||n<0)return NULL;
  char *temp=(char *)v;
  for(int i=0;i<n;i++){
    temp[i]=(char)c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
   if(!(out && in)) return 0;
   void *out0=out;
   while(n){ 
     *(char *)out=*(char *)in;
     out++;
     in++;
     n--;
   }
   return out0;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char *r1=(char *)s1;
  char *r2=(char *)s2;
  for(int i=0;i<n;i++){
    if(r1[i]>r2[i])return 1;
    if(r1[i]<r2[i])return -1;
  }
  return 0;
}
char * strchr(const char *str, int ch)
{
    while (*str && *str != (char)ch) str++;
 
    if (*str == (char)ch)
        return((char *)str);
 
    return 0;
}
#endif
