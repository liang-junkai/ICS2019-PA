#include<unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
//static int count=0;
unsigned int  choose(int i){
unsigned int  a=rand()%i;
return a;
}

// this should be enough
static char buf[65536];
void gen(const char *a){
  strcat(buf,a);
}

void append_space(){
  strcat(buf," ");
}

void is_insert(){
  if(choose(5)==0)
	append_space();
}
void gen_num(){
 /* int t=choose(2)+1;
  if(t==1){
  int num=choose(9)+1;
  char a=num+'0';
  char str[2]={a,'\0'};
  gen(str);
  }
  if(t==2){
  int num1=choose(9)+1;
  int num2=choose(9)+1;
  char a1=num1+'0';
  char a2=num2+'0';
  char str[3]={a1,a2,'\0'};
  gen(str);
  }*/
  //srand((unsigned)time(NULL));
  unsigned a=rand()%1000;
  char str[32];
  sprintf(str,"%u",a);
  gen(str);
}
void gen_rand_op(){
  //static int a=0;
  //a%=4;
  switch(choose(4)){
	case 0:gen("+");break;
	case 1:gen("-");break;
	case 2:gen("*");break;
	case 3:gen("/");break;
	default:printf("func choose() has something wrong!");
  }
}
static int depth;
static inline void gen_rand_expr() {
// printf("choose%d\n",choose(3));
//static int a=0;
//a%=10;
//int b;
//if(a>=3)b=0;
 int a=choose(3);
 if(depth<=10)depth++;
 if(depth>10)a=0;
 switch(a){
	case 0:gen_num();is_insert();break;
	case 1:gen("(");gen_rand_expr();is_insert();gen(")");break;
	case 2:gen_rand_expr();gen_rand_op();is_insert();gen_rand_expr();break;
  }
//a++;  

}
static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"#include<math.h>\n"
"int main() { "
"	unsigned int result=%s;"
"	printf(\"%%u\",result);"
"  return 0; "
"}";
int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf[0]='\0';
    depth=0;
//    strcat(buf,"1.0/0.0");
    gen_rand_expr();
   // strcpy(buf,"(4+3)*(3+5)");
//    printf("buf::%s\n",buf);
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc  /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
