#include <string.h>
#include <stdlib.h>

/*
Erros retornam 1 se forem erros operacionais
              -1 se forem erros de escrita de codigo
*/

int Erros(int V, int L){ //Erros de execucao do compilador. V = Codigo do erro. L = Linha do erro
  //0 ~ 127 Erros Operacionais
  if (V <= 127 && V >= 0){
    if (V == 0){
      printf ("Erro nao catalogado");
      return 1;
    }
    if (V == 1 && L == 0){ //Compilador normal
      printf ("\nChamada do programa: \n");
      printf("Compilador arquivoentrada\n\n");
      printf("arquivoentrada: Arquivo de entrada .cnm a ser processado (extensao requerida)\n  ");
      return 1;
    }
    if (V == 1 && L == 1){ //Compilador debug
      printf ("\nChamada do programa: \n");
      printf("Compilador arquivoentrada opcao\n\n   ");
      printf("arquivoentrada: Arquivo de entrada .cnm a ser processado (extensao requerida)\n   ");
      printf("opcao         : Mostra o debug de partes especificas do processo\n");
      printf("     1 - Mostra debug da analise lexica\n");
      printf("     2 - Mostra debug da analise sintatica\n");
      printf("     3 - Mostra debug das analises lexica e sintatica\n");
      printf("     4 - Mostra debug da analise semantica\n");
      printf("     5 - Mostra debug das analises lexica e semantica\n");
      printf("     6 - Mostra debug das analises sintatica e semantica\n");
      printf("     7 - Mostra debug das analises lexica, sintatica e semantica\n");
      return 1;
    }
    if (V == 2){
      printf ("Falha ao abrir arquivo de entrada\n");
      return 1;
    }
    if (V == 3){
      printf ("Falha ao abrir arquivo de strings\n");
      return 1;
    }
    if (V == 4){
      printf ("Falha ao abrir arquivo de codigo intermediario\n");
      return 1;
    }
    if (V == 5){
      printf ("Falha ao reabrir arquivo de strings\n");
      return 1;
    }
    if (V == 6){
      printf("Falha ao abrir o arquivo assembly\n");
      return 1;
    }
    //7 ~ 20 Reservados para erros envolvendo abertura de Arquivos
    if (V == 21){
      printf("Nao foi possivel alocar vetor auxiliar\n");
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
    if (V == 135){
      printf ("Houve um erro na chamada da funcao de verificacao de numeros\n");
      return 1;
    }
    //136 ~ 139 Reservados para erros envolvendo reconhecimento de numeros
    if (V == 140){
      printf ("Caractere invalido - Linha %i\n", L);
      return -1;
    }
    if (V == 141){
      printf ("Quebras de linha demais entre sentencas - Linha %i\n", L);
      return -1;
    }
    if (V == 142){
      printf ("Esperadas aspas para fechar caractere - Linha %i\n", L);
      return -1;
    }
  }
  //256 ~ 511 Erros Sintaticos
  if (V <= 511 && V >= 256){
    if (V == 256){
      printf("Fim da fila em momento inesperado\n");
      exit(-1);
    }
    if (V == 257){
      printf("els nao esperado - Linha %i\n", L);
      exit(-1);
    }
    if (V == 258){
      printf("nao esperado - Linha %i\n", L);
      exit(-1);
    }
    if (V == 259){
      printf("Id esperado - Linha %i\n", L);
      exit(-1);
    }
    if (V == 260){
      printf("Delimitador de sentenca esperado (;) - Linha %i\n", L-1);
      exit(-1);
    }
    if (V == 261){
      printf("Operador relacional esperado - Linha %i\n", L);
      exit(-1);
    }
    if (V == 262){
      printf("Parenteses esquerdo esperado (() - Linha %i\n", L);
      exit(-1);
    }
    if (V == 263){
      printf("Parenteses direito esperado ()) - Linha %i\n", L);
      exit(-1);
    }
    if (V == 264){
      printf("Chave esquerda esperada ({) - Linha %i\n", L);
      exit(-1);
    }
    if (V == 265){
      printf("Chave direita esperada (}) - Linha %i\n", L);
      exit(-1);
    }
    if (V == 266){
      printf("Esperado els ou inicio de expressao - Linha %i\n", L);
      exit(-1);
    }
    if (V == 267){
      printf("Valor de impressao invalido - Linha %i\n", L);
      exit(-1);
    }
    if (V == 268){
      printf("Sinal de igualdade esperado para atribuicao - Linha %i\n", L);
      exit(-1);
    }
    if (V == 269){
      printf("Construcao incorreta da atribuicao - Linha %i\n", L);
      exit(-1);
    }
    if (V == 270){
      printf("Algo deu errado na analise sintatica - Linha %i\n", L);
      exit(-1);
    }
    if (V == 271){
      printf("Arquivo Vazio\n");
      exit(-1);
    }
    if (V == 272){
      printf("Fila com provavelmente apenas um elemento\n");
      exit(-1);
    }
    if (V == 273){
      printf("String esperada para nome de arquivo - Linha %i", L);
      exit(-1);
    }
    if (V == 274){
      printf("Caractere esperado para modo de abertura de arquivo - Linha %i", L);
      exit(-1);
    }
    if (V == 275){
      printf("Numero inteiro esperado para tamanho do vetor - Linha %i", L);
      exit(-1);
    }
    if (V == 276){
      printf("Colchete direito (]) esperado para definicao do tamanho - Linha %i", L);
      exit(-1);
    }
    /*if (V == 27){
      printf(" - Linha %i", L);
      exit(-1);
    }*/
  }
  //512 ~ 768 Erros Semanticos e de geracao de codigo intermediario
  if (V >= 512 && V <= 768){
    if (V == 512){
      printf("Variavel nao declarada - Linha %i\n", L);
      exit(-1);
    }
    if (V == 513){
      printf("brk fora de um laco de repeticao - Linha %i\n", L);
      exit(-1);
    }
    if (V == 514){
      printf("Nao permitido uso do operador com float - Linha %i\n", L);
      exit(-1);
    }
    if (V == 515){
      printf("Atribuicao a int de operacoes sobre flt - Linha %i\n", L);
      exit(-1);
    }
    if (V == 516){
      printf("Variavel nao inicializada - Linha %i\n", L);
    }
    if (V == 517){
      printf("O contador de condicionais e lacos excedeu o limite - Linha %i", L);
      exit(-1);
    }
    if (V == 518){
      printf("Nenhum arquivo aberto - Linha %i", L);
      exit(-1);
    }
    if (V == 519){
      printf("Arquivo aberto em modo escrita nao pode ser lido - Linha %i", L);
      exit(-1);
    }
    if (V == 520){
      printf("Arquivo aberto em modo leitura nao pode sofrer escritas - Linha %i", L);
      exit(-1);
    }
    if (V == 521){
      printf("Arquivo nao pode ser aberto em condicional ou laco de repeticao - Linha %i", L);
      exit(-1);
    }
    if (V == 522){
      printf("Modo de abertura nao reconhecido - Linha %i", L);
      exit(-1);
    }
    if (V == 523){
      printf("Tamanho do vetor nao pode ser menor ou igual a zero - Linha %i", L);
      exit(-1);
    }
    if (V == 524){
      printf("Variavel declarada como vetor deve ser acessada como vetor - Linha %i", L);
      exit(-1);
    }
    /*if (V == 52){
      printf(" - Linha %i", L);
      exit(-1);
    }*/
  }
  /*
  if (V == 142){
    printf (" - Linha %i\n", L);
    return -1;
  }
  */
  //769 ~ 1024 Erros do Compilador Debugger
  if (V >= 769 && V <= 1024){
    if (V == 769){
      printf (" - Linha %i\n", L);
      return -1;
    }
  }
  printf ("Problemas com processamento de erros\n");
  exit(1);
}
