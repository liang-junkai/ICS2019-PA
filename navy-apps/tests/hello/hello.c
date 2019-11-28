#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  //write(1,"hello ljk\n",10);
  while (1) {
    j ++;
    if (j == 10000) {
      printf("Hello World from Navy-apps for the %d time!\n",j);
      j = 0;
    }
  }
  return 0;
}
