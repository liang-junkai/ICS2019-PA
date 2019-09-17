#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
extern int number_ljk(char *arg,int dec);
static int cmd_si(char *args){
  char* arg=strtok(NULL," ");
  if(arg==NULL){
	cpu_exec(1);
}
  else{
	int count_ljk=number_ljk(arg,10);
	cpu_exec(count_ljk);
}
return 0; 
}
extern void isa_reg_display();
static int cmd_info(char *args){
	char *arg=strtok(NULL," ");
	if(arg==NULL){
		printf("Use r or w as a info!");
		return 0;
}
	else if(strcmp(arg,"r")==0){
		isa_reg_display();
		return 0;
}
	else if(strcmp(arg,"w")==0){
		printf("we will define info[w] later\n");
		return 0;
}
	else{
		printf("Please use info [r][w] correctly! for more info, input help info\n");
		return 0;
}
}

extern int mulptily_ljk(int i,int j);
int number_ljk(char *arg,int dec){
  int result=0;
  int i=0;
  for(i=0;i<strlen(arg);i++){
	if(arg[i]>='0'&&arg[i]<='9'){
	result+=(arg[i]-'0')*mulptily_ljk(dec,strlen(arg)-i-1);
}
	else if(arg[i]>='a'&&arg[i]<='z'){
		if(arg[i]-'a'+10>=dec){
			printf("wrong input\n");
			return -1;
}
	result+=(arg[i]-'a'+10)*mulptily_ljk(dec,strlen(arg)-i-1);
}
/*	else{
		printf("wrong number input");
		return -1;
}*/
}
  return result;
}
int mulptily_ljk(int i,int j){
	int k=0,result=1;
	for(k=0;k<j;k++){
		result*=i;
}
return result;
}

extern uint32_t paddr_read(paddr_t addr,int len);
static int cmd_x(char *args){
  char *arg=strtok(NULL," ");
  int len_ljk=number_ljk(arg,10);
  arg=strtok(NULL," ");
  int i=0;
  for(i=0;i<strlen(arg)-2;i++){
  arg[i]=arg[i+2];
}
  arg[i]='\0';
  paddr_t addr=number_ljk(arg,16);
  printf("0x%s:\t",arg);
  for(i=1;i<=4*len_ljk;i++){
	if(i%4!=0){
		printf("%-4x ",paddr_read(addr+i-1,1));
}
	else{
		printf("%-4x ",paddr_read(addr+i-1,1));
		printf("\t");
		int j;
		for(j=i-3;j<=i;j++)
			printf("%-4d ",paddr_read(addr+j-1,1));
		printf("\n");
		if(i==4*len_ljk){
			printf("\n");
}
		else{
			printf("0x%x:\t",addr+i);
}
}
}
 // printf("0x%s:\t%x\n",arg,paddr_read(addr,len_ljk));
return 0;
}

static int cmd_p(char *args){
  char *arg=args;
  if(arg[0]=='p'&&arg[1]==' '){
	int i=0;
	for(i=0;i<strlen(arg)-3;i++){
        	arg[i]=arg[i+2];
        }
	arg[i]='\0';
  }
  else{
	printf("Please Use p [expr] correctly!\n");
	return 0;
}
bool success1=false;
 bool *success;
 success=&success1;
 uint32_t result=expr(arg,success);
 printf("%u\n",result);
 return 0;
}


static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","Continue the progrom with a certain step you have given: si [N]",cmd_si},
  /* TODO: Add more commands */
  {"info","info [r][w], use r to type the values of the 32_bytes registers, use w know the info of the watchpoint",cmd_info},
  {"x","Use x [N] [addr] to know the value in a certain address",cmd_x},
  {"p","Use p [expr] to caculate the expr you have given",cmd_p}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))



static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
