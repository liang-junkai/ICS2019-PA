#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL,*tail_head=NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP *new_wp(){
  if(free_==NULL)assert(free_!=NULL);
  if(head==NULL){
	WP *tail_free=free_;
	for(;tail_free->next->next!=NULL;tail_free=tail_free->next);
	tail_head=head=tail_free->next;
	tail_free->next=NULL;
	return tail_head; 
  }
  else{
	WP *tail_free=free_;
	for(;tail_free->next->next!=NULL;tail_free=tail_free->next);
	tail_head->next=tail_free->next;
	tail_head=tail_head->next;
	tail_free->next=NULL;
	return tail_head;
  } 
}

void free_wp(WP *wp){

}
void watchpoint_display(){
  int i=1;
  printf("%-10s%-20s%-15s%-15s\n","NUM","EXPR","VALUE","0x_VALUE");
  for(WP* p=head;p!=NULL;p=p->next){
	bool success=false;
	printf("%-10d%-20s%-15u0x%-15x\n",i,p->str,expr(p->str,&success),expr(p->str,&success));
	i++;
  }
}
