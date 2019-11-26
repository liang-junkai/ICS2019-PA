#include <unistd.h>
#include <stdio.h>

int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  write(1,"hello world!\n",13);
  //write(1,"hello ljk\n",10);
  while (1) {
    j ++;
    if (j == 10000) {
      write(1,"Hello World from Navy-apps for the time!\n",41);
      j = 0;
    }
  }
  return 0;
}
