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
  context_kload(&pcb[0], (void *)hello_fun);
  printf("hello:%x\n",(unsigned)hello_fun);
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
  current=&pcb[0];
  /*static int fd=-1;
  if(fd==-1){
    fd=fs_open("/dev/events",0,0);
  }
  char info[25],con[10]="kd F\0\0";
  fs_read(fd,info,25);
  int i;
  for(i=1;i<5;++i){
    con[4]='0'+i;//4 for "kd F"
    if(!strncmp(info,con,5)){
      switch(i){
          case 1:case 2:case 3:
            printf("switch between hello and %s\n",program[i]+5);//+5 for "/bin/"
            fg_pcb=i;
            break;
          case 4:
            printf("reload pcb[0]\n");
            context_uload(&pcb[0], program[0]);
            break;
        }
        break;
      }
    }*/
    printf("cp: 0x%x\n",current->cp->eip);
    return current->cp;
}
