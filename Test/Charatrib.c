#include <stdio.h>
#include <stdlib.h>

FILE* IN0;
int j;

int main(int argc, char const *argv[]) {
  char A = '!';
  int i = A;
  printf("A = %c, i = %i, i = %c\n", A, i, i);
  IN0 = fopen("Saida.txt", "w");
  if (IN0 == NULL){
    return 1;
  }
  printf("Arquivo aberto\n");
  j = fprintf(IN0, "O valor de A = %c\nE o de i = %i - %c", A, i, i);
  printf("fprintf retornou %i\n", j);
  return 0;
}
