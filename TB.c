#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
  char A[20];
  strcpy(A, argv[1]);
  printf("+%s+%s+\n", A, argv[1]);
  return 0;
}
