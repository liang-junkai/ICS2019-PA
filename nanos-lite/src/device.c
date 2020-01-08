#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //_yield();
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
int fg_pcb=1;
size_t events_read(void *buf, size_t offset, size_t len) {
  //_yield();
  int key=read_key();
  
	if (key == _KEY_NONE) {
		unsigned long t = uptime();
		sprintf(buf, "t %d\n", t);
	}
	else {
    if(key&0x8000){
      //printf("%x\n",key);
      sprintf(buf, "ku %s\n", keyname[key&0x7fff]);
      if (strcmp("F1", keyname[key&0x7fff])==0) {
        //assert(0);
        fg_pcb = 1;
      }  
      else if (strcmp("F2", keyname[key&0x7fff])==0)
        fg_pcb = 2;
      else if (strcmp("F3", keyname[key&0x7fff])==0)
        fg_pcb = 3;
    }
    else{
      sprintf(buf, "kd %s\n", keyname[key&0x7fff]);
    }
  }
	return strlen(buf);
  //printf("%d\n",key);
  //printf("%d\n",len);
  //return sprintf(buf,"t %d\n",uptime());
  /*if(key!=0)printf("key\n");
  if(key==0){
    return sprintf(buf,"t %d\n",uptime());
  }else{
    //printf("hit\n");
    return sprintf(buf,"k%c %s\n",key&0x8000?'u':'d',keyname[key&0x7fff]);
  }*/
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  if(len+offset>sizeof(dispinfo)){
    len= sizeof(dispinfo)-offset;
  }
  memcpy(buf, dispinfo + offset, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int i, x, y;
  offset>>=2;
  y = offset / screen_width();
  x = offset - y * screen_width();
  for(i=0;i<len/4;++i){
    draw_rect((uint32_t*)buf,x,y,len/4-i,1);
    i+=screen_width()-x;
    x=0;++y;
  }
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
  //return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
