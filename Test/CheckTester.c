#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "Erros.h"
#include "Checker.h"

double tempo;
clock_t inicio, fim;

int main(){ //Testa cada um dos caracteres imprimiveis
  int B;
  char A;
  inicio = clock();
  for (A = 33;A < 127; A++){
    B = QueEso(A);
    printf(".%c.-%i\n",A,B);
  }
  fim = clock();
  tempo = (double) (((fim - inicio) + 0.0) / CLOCKS_PER_SEC);
  printf("Tempo necessario: %lf", tempo);
  return 0;
}
