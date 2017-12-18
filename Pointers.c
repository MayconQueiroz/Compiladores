#include <stdio.h>
#include <stdlib.h>

int i;

int Teste(int A){
  int B;
  B = i+1;
  i++;
  printf("B = %i\n", B);
  if (B > 10){
    exit(0);
  }
  B = Teste(B);
  return B;
}

int main(int argc, char const *argv[]) {
  int A = 0;
  A = Teste(A);
  printf("A = %i\n", A);
  A = Teste(A);
  printf("A = %i\n", A);
  A = Teste(A);
  printf("A = %i\n", A);
  A = Teste(A);
  printf("A = %i\n", A);
  return 0;
}
