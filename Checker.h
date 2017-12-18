#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Verifica tipos das coisas, nada mais por enquanto
*/

//0 - Numero. 1 - Letra. 2 - Simbolo. 3 - Invalido
int QueEso(char C){
  if (C == '0') { return 0; }
  if (C == '1') { return 0; }
  if (C == '2') { return 0; }
  if (C == '3') { return 0; }
  if (C == '4') { return 0; }
  if (C == '5') { return 0; }
  if (C == '6') { return 0; }
  if (C == '7') { return 0; }
  if (C == '8') { return 0; }
  if (C == '9') { return 0; }
  if (C == 'a') { return 1; }
  if (C == 'b') { return 1; }
  if (C == 'c') { return 1; }
  if (C == 'd') { return 1; }
  if (C == 'e') { return 1; }
  if (C == 'f') { return 1; }
  if (C == 'g') { return 1; }
  if (C == 'h') { return 1; }
  if (C == 'i') { return 1; }
  if (C == 'j') { return 1; }
  if (C == 'k') { return 1; }
  if (C == 'l') { return 1; }
  if (C == 'm') { return 1; }
  if (C == 'n') { return 1; }
  if (C == 'o') { return 1; }
  if (C == 'p') { return 1; }
  if (C == 'q') { return 1; }
  if (C == 'r') { return 1; }
  if (C == 's') { return 1; }
  if (C == 't') { return 1; }
  if (C == 'u') { return 1; }
  if (C == 'v') { return 1; }
  if (C == 'w') { return 1; }
  if (C == 'x') { return 1; }
  if (C == 'y') { return 1; }
  if (C == 'z') { return 1; }
  if (C == 'A') { return 1; }
  if (C == 'B') { return 1; }
  if (C == 'C') { return 1; }
  if (C == 'D') { return 1; }
  if (C == 'E') { return 1; }
  if (C == 'F') { return 1; }
  if (C == 'G') { return 1; }
  if (C == 'H') { return 1; }
  if (C == 'I') { return 1; }
  if (C == 'J') { return 1; }
  if (C == 'K') { return 1; }
  if (C == 'L') { return 1; }
  if (C == 'M') { return 1; }
  if (C == 'N') { return 1; }
  if (C == 'O') { return 1; }
  if (C == 'P') { return 1; }
  if (C == 'Q') { return 1; }
  if (C == 'R') { return 1; }
  if (C == 'S') { return 1; }
  if (C == 'T') { return 1; }
  if (C == 'U') { return 1; }
  if (C == 'V') { return 1; }
  if (C == 'W') { return 1; }
  if (C == 'X') { return 1; }
  if (C == 'Y') { return 1; }
  if (C == 'Z') { return 1; }
  if (C == '&') { return 2; }
  if (C == '|') { return 2; }
  if (C == '!') { return 2; }
  if (C == '^') { return 2; }
  if (C == '<') { return 2; }
  if (C == '>') { return 2; }
  if (C == ';') { return 2; }
  if (C == '+') { return 2; }
  if (C == '-') { return 2; }
  if (C == '*') { return 2; }
  if (C == '/') { return 2; }
  if (C == '%') { return 2; }
  if (C == '=') { return 2; }
  if (C == '{') { return 2; }
  if (C == '}') { return 2; }
  if (C == '(') { return 2; }
  if (C == ')') { return 2; }
  if (C == ' ') { return 2; }
  if (C == '#') { return 2; }
  if (C == '\n') { return 2; }
  if (C == '\t') { return 2; }
  if (C == '\"') { return 2; }
  if (C == '\'') { return 2; }
  return 3;
}

//0 - Numero Representavel. 1 - Numero nao representavel. 2 - Erro de chamada
int NumeroCorecto(char* N, int T){
  int A, i, j;
  float B;
  char C[256];
  i = 0;
  j = 0;
  if (T == 0){ //Numero Decimal
    B = atof(N);
    sprintf(C, "%g", B);
    while (i == 0){ //Compara a string do numero original e o que oi convertido para string, se der diferente retorna erro
      if (N[j] != C[j] || j == 255){
        return 1;
      }
      if (N[j] == '\0'){ //Fim da string (So precisa de um)
        i = 1;
      }
      j++;
    }
    return 0;
  }
  if (T == 1){ //Numero Inteiro
    A = atoi(N);
    sprintf(C, "%d", A);
    while (i == 0){ //Compara a string do numero original e o que o convertido para string, se der diferente retorna erro
      if (N[j] != C[j] || j == 255){
        return 1;
      }
      if (N[j] == '\0'){ //Fim da string (So precisa de um)
        i = 1;
      }
      j++;
    }
    return 0;
  }
  return 2;
}

//0 - Caractere imprimivel. 1 - Caractere nao imprimivel
int impCaractere(char A){
  if (A == 32|| A == 33|| A == 35|| A == 36|| A == 37|| A == 38|| (A > 39 && A < 127)){
    return 0;
  }
  return 1;
}
