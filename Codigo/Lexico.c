#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"
#include "Checker.h"

/**
* Recebe codigo.cnm e escreve em arquivo codigo.lnm
*/

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

char U; //Caractere sendo lido
char L[256]; //String para atribuicao de identificadores e numeros
int Pl; //Posicao da linha (Contado com \n)

//Declaracao de funcoes usadas a posteriori
int leId();
int leNumero();
void optok();

int main(int argc, char *argv[]){
  int I, R;
  if (argc != 3){
		return Perros(1);
	}

  //Abertura Arquivos
  IN = fopen(argv[1], "r");
  OUT = fopen(argv[2], "w");
  if (IN == NULL){
    return Perros(2);
  }
  if (OUT == NULL){
    return Perros(3);
  }

  //Inicializacoes
  Pl = 1;

  //Leitura Codigo Fonte
  while (!feof(IN)){
    fscanf (IN, "%c", &U);
    I = QueEso(U); //Funcao que retorna codigo de caractere (vide Checker.h)
    if (I == 0){ //Numero (Inteiro ou decimal)
      R = leNumero();
      if (R == 1){ //Numero muito extenso
        return Lerros(131, Pl);
      }
      if (R == 2){ //Dois pontos encontrados em sequencia (nao se sabe onde quebrar o numero)
        return Lerros(132, Pl);
      }
      fprintf(OUT, "\n");
    } else if (I == 1){ //Caractere (Id ou Palavra reservada)
      R = leId();
      if (R == 3){ //Caractere invalido
        return Lerros(128, Pl);
      } else if (R == 4){ //String muito longa
        return Lerros(129, Pl);
      }
      fprintf(OUT, "\n");
    } else if (I == 2){ //Simbolo
      if (U == '\"'){ //Inicio de string
        fprintf(OUT, "%c", U);
        fscanf (IN, "%c", &U);
        if (U == '\"'){ //Aspas imediatamente apos outras (Ilegal)
          return Lerros(130, Pl);
        }
        while (U != '\"' && !feof(IN)){ //Imprime toda a string no arquivo de saida
          fprintf(OUT, "%c", U);
          fscanf (IN, "%c", &U);
        }
        fprintf(OUT, "%c", U); //Imprime as ultimas aspas no arquivo de saida
        fprintf(OUT, "\n");
      } else if (U == '\n'){ //Quebra de linha
        Pl++;
      } else if (U == '#'){ //Comentario
        while (U != '\n' && !feof(IN)){ //Enquanto nao for o fim do arquivo ou quebra de linha
          fscanf (IN, "%c", &U);
        }
        Pl++;
      } else { //Qualquer outra coisa (Simbolos reconhecidos)
        optok();
      }
    } else {
      printf ("\"%c\" ", U);
      return Lerros(128, Pl);
    }
  }
  return 0;
}

//Separa os tokens que interessam
void optok(){
  int i = 1;
  L[0] = U;
  if (U == '<'){ //<< ou <=
    fscanf (IN, "%c", &U);
    if (U == '<' || U == '='){
      L[1] = U;
      i = 2;
    }
  }else if (U == '>'){ //>= ou >>
    fscanf (IN, "%c", &U);
    if (U == '>' || U == '='){
      L[1] = U;
      i = 2;
    }else {
      fseek(IN, -1, SEEK_CUR); //Retorna um caractere
    }
  }else if (U == '!'){ //! ou !=
    fscanf (IN, "%c", &U);
    if (U == '='){
      L[1] = '=';
      i = 2;
    }else {
      fseek(IN, -1, SEEK_CUR); //Retorna um caractere
    }
  }else if (U == '='){ //= ou ==
    fscanf (IN, "%c", &U);
    if (U == '='){
      L[1] = '=';
      i = 2;
    }else {
      fseek(IN, -1, SEEK_CUR); //Retorna um caractere
    }
  }
  L[i] = '\0'; //Termina a string
  //Operadores Logicos
  if (!strcmp(L, "&") || !strcmp(L, "|") || !strcmp(L, "!") || !strcmp(L, "^") || !strcmp(L, "<<") || !strcmp(L, ">>")){
    fprintf(OUT, "[ol, %s]\n", L);
  }
  //Operadores Aritmeticos
  else if (!strcmp(L, "+") || !strcmp(L, "-") || !strcmp(L, "*") || !strcmp(L, "/") ||  !strcmp(L, "%%")){
    fprintf(OUT, "[oa, %s]\n", L);
  }
  //Operadores Relacionais
  else if (!strcmp(L, "==") || !strcmp(L, "!=") || !strcmp(L, "<=") || !strcmp(L, ">=") || !strcmp(L, "<") || !strcmp(L, ">")){
    fprintf(OUT, "[or, %s]\n", L);
  }
  //Atribuicao
  else if (!strcmp(L, "=")){
    fprintf(OUT, "[at, %s]\n", L);
  }
  else {
    //Nao propaga a impressao dos caracteres "whitespace"
    if (U != '\n' && U != ' ' && U != '\t'){
      fprintf(OUT, "%s\n", L);
    }
  }
}

//Se for qualquer uma das palavras reservadas retorna 1
int checkToken(){
  if (strcmp(L, "chr") == 0 || strcmp(L, "int") == 0 || strcmp(L, "flt") == 0 || strcmp(L, "brk") == 0 || strcmp(L, "scn") == 0 || strcmp(L, "iff") == 0 || strcmp(L, "els") == 0 || strcmp(L, "for") == 0 || strcmp(L, "whl") == 0 || strcmp(L, "prt") == 0){
    return 1;
  }
  return 0;
}

//Le caracteres ou digitos ate encontrar outra coisa e reconhece o que leu
int leId(){
  int C = 0, i = 0, F = 0;
  while ((C == 0 || C == 1) && !feof(IN)){ //Le enquanto forem caracteres ou digitos (assumindo que o primeiro foi caractere)
    L[i] = U;
    fscanf (IN, "%c", &U);
    C = QueEso(U); //vide Checker.h
    i++;
    if (i == 255){ //String muito longa
      return 4;
    }
  }
  L[i] = '\0'; //Termina a string
  if (i == 3){
    F = checkToken(); //Verifica se e uma das palavras reservadas da linguagem
    if (F == 1){
      fprintf(OUT, "%s", L);
    } else {
      fprintf(OUT, "[id, %s]", L);
    }
  } else {
    fprintf(OUT, "[id, %s]", L);
  }
  fseek(IN, -1, SEEK_CUR); //Retorna um caractere
  return C;
}

int leNumero(){
  int C = 0, i = 0, F = 0;
  while ((C == 0 || U == '.') && !feof(IN)){ //Le enquanto forem digitos ou ponto (assumindo que o primeiro foi digito)
    L[i] = U;
    fscanf (IN, "%c", &U);
    if (U == '.' && F == 0){
      F = 1;
    } else if (U == '.' && F == 1){ //Se ler dois pontos
      return 2;
    }
    C = QueEso(U); //vide Checker.h
    i++;
    if (i == 255){ //Numero muito longo
      return 1;
    }
  }
  L[i] = '\0'; //Termina numero
  if (F == 1){ //Decimal
    fprintf(OUT, "[nd, %s]", L);
  } else { //Inteiro
    fprintf(OUT, "[ni, %s]", L);
  }
  fseek(IN, -1, SEEK_CUR); //Retorna um caractere
  return 0;
}
