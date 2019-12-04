#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int i;
  for(i=0;i<len;++i){
    _putc(*(char*)(buf+i));
  }
  return i;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key=read_key();
  //printf("%d\n",key);
  //printf("%d\n",len);
  //return sprintf(buf,"t %d\n",uptime());
  if(key!=0)printf("key\n");
  if(key==0){
    return sprintf(buf,"t %d\n",uptime());
  }else{
    //printf("hit\n");
    return sprintf(buf,"k%c %s\n",key&0x8000?'u':'d',keyname[key&0x7fff]);
  }
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
