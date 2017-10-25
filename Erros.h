#include <string.h>
#include <stdlib.h>

/*
Erros retornam  1 se forem erros operacionais
      retornam -1 se forem erros de escrita de codigo
*/

int Erros(int V, int L){ //Erros de execucao do compilador. V = Codigo do erro. L = Linha do erro
  //0 ~ 127 Erros Operacionais
  if (V <= 127 && V >= 0){
    if (V == 0){
      printf ("Erro nao catalogado");
      return 1;
    }
    if (V == 1){
      printf ("Quantidade invalida de argumentos\n");
      printf ("Chamada do programa: ");
      printf("[programa] [arquivo entrada]\n  programa       :\n    Windows: Lexico.exe\n    Linux  : ./Lexico.x\n  ");
      printf("arquivo entrada: Arquivo de entrada .cnm a ser processado\n  ");
      return 1;
    }
    if (V == 2){
      printf ("Falha ao abrir arquivo de entrada\n");
      return 1;
    }
    if (V == 3){
      printf ("Falha ao abrir arquivo de saida\n");
      return 1;
    }
  }
  //128 ~ 255 Erros Lexicos
  if (V >= 128 && V <= 255){
    if (V == 128){
      printf ("Simbolo invalido - Linha %i\n", L);
      return -1;
    }
    if (V == 129){
      printf ("String muito longa - Linha %i\n", L);
      return -1;
    }
    if (V == 130){
      printf ("Aspas vazias - Linha %i\n", L);
      return -1;
    }
    if (V == 131){
      printf ("Numero muito extenso - Linha %i\n", L);
      return -1;
    }
    if (V == 132){
      printf ("Dois pontos encontrados em sequencia - Linha %i\n", L);
      return -1;
    }
    if (V == 133){
      printf ("Ausencia de digitos apos o ponto - Linha %i\n", L);
      return -1;
    }
    if (V == 134){
      printf ("Numero nao representavel - Linha %i\n", L);
      return -1;
    }
    //135 ~ 139 Reservados para erros envolvendo reconhecimento de numeros
    if (V == 140){
      printf ("Caractere invalido - Linha %i\n", L);
      return -1;
    }
    if (V == 141){
      printf ("Quebras de linha demais entre sentencas - Linha %i\n", L);
      return -1;
    }
  }
  //256 ~ 511 Erros Sintaticos
  if (V <= 511 && V >= 256){
    if (V == 256){
      printf("Fim do programa em momento inesperado\n");
      exit(1);
    }
    if (V == 257){
      printf("els nao esperado - Linha %i\n", L);
      exit(1);
    }
    if (V == 258){
      printf("nao esperado - Linha %i\n", L);
      exit(1);
    }
    if (V == 259){
      printf("Id esperado - Linha %i", L);
      exit(1);
    }
    if (V == 260){
      printf("Delimitador de sentenca esperado (;) - Linha %i", L);
      exit(1);
    }
    if (V == 261){
      printf("Operador relacional esperado - Linha %i", L);
      exit(1);
    }
    if (V == 26){
      printf(" - Linha %i", L);
      exit(1);
    }
  }
  /*
  if (V == 142){
    printf (" - Linha %i\n", L);
    return -1;
  }
  */
  printf ("Problemas com processamento de erros\n");
  return 1;
}
