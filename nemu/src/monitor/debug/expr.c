#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_PLUS,TK_EQ,TK_SUB,
  TK_DIV,TK_LBRA,TK_RBRA,TK_NUM,TK_MUL
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
  {"-",TK_SUB},//sub
  {"/",TK_DIV},//chu
  {"\\(",TK_LBRA},
  {"\\)",TK_RBRA},
  {"[1-9]+",TK_NUM},
  {"\\*",TK_MUL}
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

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
//     printf("ljk::%s\n",e);
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
//printf("ljk::%d\n",position);
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
	tokens[nr_token].type=rules[i].token_type;
if(tokens[nr_token].type!=TK_NOTYPE){
	int ljk=0;
	for(ljk=0;ljk<substr_len;ljk++){
		tokens[nr_token].str[ljk]=e[position-substr_len+ljk];
	}
	tokens[nr_token].str[substr_len]='\0';
printf("ljk::%s\n",tokens[nr_token].str);
//printf("ljk::%s\n",tokens[nr_token].str);
	nr_token++;}
else{position--;}
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

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  int i=0;
  for(i=0;i<=nr_token;i++){
  printf("%s\n",tokens[i].str);
  }
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
  return 0;
}
