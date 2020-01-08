#include "proc.h"

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
//static int fg_pcb=1;
void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
  }
}
void context_kload(PCB *pcb, void *entry);
void naive_uload(PCB *pcb, const char *filename);
void context_uload(PCB *pcb, const char *filename) ;
void init_proc() {
  //switch_boot_pcb();

  //Log("Initializing processes...");
  //context_kload(&pcb[0], (void *)hello_fun);
  //context_uload(&pcb[1],"/bin/init");
  context_uload(&pcb[0],"/bin/pal");
  switch_boot_pcb();
  // load program here
  //naive_uload(NULL,"/bin/init");

}
char *program[]={
  "/bin/hello",
  "/bin/pal",
  "/bin/dummy",//For exit
  "/bin/init"
};
size_t fs_read(int fd,void *buf,size_t len);
size_t fs_open(const char* pathname,int flags,int mode);
_Context* schedule(_Context *prev) {
  current->cp=prev;
  current=current==&pcb[0]? &pcb[1]:&pcb[0];
  return current->cp;
}
