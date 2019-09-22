#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_PLUS,TK_EQ,TK_SUB,
  TK_DIV,TK_LBRA,TK_RBRA,TK_HXNUM,TK_NUM,TK_MUL,TK_NONE,
  TK_NEQ,TK_AND
 /* TODO: Add more token types */ 
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_PLUS},         // plus
  {"==", TK_EQ},// equal
  {"!=",TK_NEQ},
  {"-",TK_SUB},//sub
  {"/",TK_DIV},//chu
  {"\\(",TK_LBRA},
  {"\\)",TK_RBRA},
  {"0x[0-9]*[a-z]*[A-Z]*",TK_HXNUM},
  {"[0-9]+",TK_NUM},
  {"\\*",TK_MUL},
  {"&&",TK_AND}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Theeefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;


void init_tokens_ljk(){
  int i;
  for(i=0;i<=nr_token;i++){
	tokens[i].type=TK_NONE;
	tokens[i].str[0]='\0';
}
}

bool check_parentheses(uint32_t p,uint32_t q){
  if(q<p)return false;
//  printf("p: %s  q:  %s\n",tokens[p].str,tokens[q].str);
  if(strcmp(tokens[p].str,"(")!=0||strcmp(tokens[q].str,")")!=0){
//	printf("1\n");
	return false;

  }
  int position[32]={0};
  int i=0;
  for(i=0;i<=q-p;i++){
	if(strcmp(tokens[p+i].str,"(")==0)position[i]=-1;
	if(strcmp(tokens[p+i].str,")")==0)position[i]=1;
  }
  int count=0;
  for(i=0;i<=q-p;i++){
	count+=position[i];
  }
  if(count!=0){/*printf("2\n");*/return false;}
  for(i=0;i<=q-p;i++){
	count+=position[i];
	if(count==0&&i!=q-p)
		{/*printf("3\n");*/ return false;} 
  }
  return true;
}
extern int number_ljk(char*arg,int dec);
uint32_t eval(int p,int q){
/*int ljk=p;
for(ljk=p;ljk<=q;ljk++){
printf("%s",tokens[ljk].str);
}
printf("\n");*/
  if(p>q){
	printf("bad expression\n");
	return -1;
}
  else if(p==q){
  if(tokens[p].type==TK_NUM){
	return number_ljk(tokens[p].str,10);}
  else if(tokens[p].type==TK_HXNUM){
	int i=0;
	char str[20];
	for(i=0;i<strlen(tokens[p].str)-2;i++){
		str[i]=str[i+2];
	}
	printf("%s\n",str);
	str[i]='\0';
	return number_ljk(str,16);
  }
  else {printf("bad expression2\n");return 0;}
}
  else if(check_parentheses(p,q)==true){
  	p++;
  	q--;
  	return eval(p,q);
}
  else{
	int position[32]={0};
	int i=0;
	for(i=0;i<=q-p;i++){
		switch(tokens[p+i].type){
			case TK_PLUS:position[i]=1;break;
			case TK_SUB:position[i]=1;break;
			case TK_MUL:position[i]=2;break;
			case TK_DIV:position[i]=2;break;
			case TK_EQ:position[i]=2;break;
			case TK_NEQ:position[i]=2;break;
			case TK_AND:position[i]=2;break;
		}
	}
	int j=0,m=0;
	for(i=0;i<q-p;i++){
	for(j=i+1;j<=q-p;j++){
		if(check_parentheses(p+i,p+j)==true){
			for(m=i;m<=j;m++)position[m]=0;
			}	
		}
	}
        int min=3;
	for(i=0;i<=q-p;i++){
		if(position[i]!=0&&min>position[i])min=position[i];
	}
	if(min==0){printf("bad expression3\n");return 0;}
	for(i=q-p;i>=0;i--){
		if(position[i]==min){
			switch(tokens[p+i].type){
				case TK_PLUS:return eval(p,p+i-1)+eval(p+i+1,q);
				case TK_SUB:return eval(p,p+i-1)-eval(p+i+1,q);
				case TK_MUL:return eval(p,p+i-1)*eval(p+i+1,q);
				case TK_DIV:return eval(p,p+i-1)/eval(p+i+1,q);
				case TK_EQ:return eval(p,p+i-1)==eval(p+i+1,q);
				case TK_NEQ:return eval(p,p+i-1)!=eval(p+i+1,q);
				case TK_AND:return eval(p,p+i-1)&&eval(p+i+1,q);
			}
		}
	}	
    }
  printf("bad expression4\n");
  return 0;
}



static bool make_token(char *e) {
  int position = 0;
  int i=0;
  regmatch_t pmatch;
//printf("ljk::e%s\n ",e);
  nr_token = 0;
//printf("ljk_no_token%d\n",nr_token);
  while (e[position] != '\0') {
//     printf("ljk::%s\n",e);
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        //char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
//printf("ljk::%d\n",position);
/*        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);*/
        position += substr_len;
//printf("ljk_position::%d\n",position);
	tokens[nr_token].type=rules[i].token_type;
if(tokens[nr_token].type!=TK_NOTYPE){
	int ljk=0;
	for(ljk=0;ljk<substr_len;ljk++){
		tokens[nr_token].str[ljk]=e[position-substr_len+ljk];
	}
	tokens[nr_token].str[substr_len]='\0';
//printf("ljk_str::%s\n",tokens[nr_token].str);
//printf("ljk::%s\n",tokens[nr_token].str);
	nr_token++;}
else{tokens[nr_token].type=TK_NONE;}
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

   /*     switch (rules[i].token_type) {
         default: TODO();
        }*/

        break;
      }
    }
//printf("i::%d  NR_REGEX  %ld\n",i,NR_REGEX); 
   if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
	return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (make_token(e)==false) {
    *success = false;
    init_tokens_ljk();
    return 0;
  }
  *success=true;
  uint32_t t=eval(0,nr_token-1);
 // for(i=0;i<nr_token;i++)
//	printf("%s\n",tokens[i].str);
//  printf("ljk::checkout  %d\n",check_parentheses(0,nr_token-1));
  init_tokens_ljk();
  return t;
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
}
