#include<stdio.h>
#include<math.h>
int main(){
  float x=1/(float)0;
  if(!isfinite(x)){
	printf("division by zero\n");
  }
  else{
	printf("%f",x);
  }
  return 0;
}
