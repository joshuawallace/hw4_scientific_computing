#include <stdio.h>
#include <stdlib.h>

void  initial_message(char *name){
  printf("Usage: %s <nx>\n",name);
  printf("  nx:    grid size on a side\n         final grid will be 2-d sized nx by nx\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc!=2) {
    initial_message(argv[0]);
  }



  return 0;
}
