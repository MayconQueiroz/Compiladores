#include <string.h>
#include <stdlib.h>

/*
Erros retornam 1 se forem erros operacionais
      retornam -1 se forem erros de escrita de codigo
*/

//0 ~ 127
int Perros(int V){ //Erros de execucao do compilador. V = Codigo do erro
  if (V == 0){
    printf ("Erro nao catalogado");
    return 1;
  }
  if (V == 1){
    printf ("Quantidade invalida de argumentos\n");
    printf ("Chamada do programa: ");
    printf("[programa] [arquivo entrada] [arquivo saida]\n  programa       :\n    Windows: Lexico.exe\n    Linux  : ./Lexico.x\n  ");
    printf("arquivo entrada: Arquivo de entrada .cnm a ser processado\n  ");
    printf("arquivo saida  : Arquivo de saida .lnm a ser escrito");
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

//128 ~ 255
int Lerros(int V, int L){ //Erros Lexicos encontrados no codigo. V = Codigo do erro; L = Linha em que o erro ocorreu
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
  /*
  if (V == 133){
    printf (" - Linha %i\n", L);
    return -1;
  }
  */
}
