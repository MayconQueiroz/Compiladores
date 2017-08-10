#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"
#include "Checker.h"

/**
* Recebe codigo.cnm e escreve em arquivo codigo.lnm
*/

FILE *IN;
FILE *OUT;

char U; //Caractere sendo lido
char L[255]; //String para atribuicao de identificadores
int Pl; //Posicao da linha (Contado com \n)

int leId();
int leNumero();

int main(int argc, char *argv[]){
  int I, R;
  if (argc != 3){
		return Perros(1);
	}

  //Arquivos
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
    //printf("-%c-\n", U);
    I = QueEso(U); //Funcao que retorna codigo de caractere (vide Checker.h)
    if (I == 0){ //Numero (Inteiro ou decimal)
      R = leNumero();
      if (R == 1){ //Numero muito extenso
        return Cerros(131, Pl);
      }
      if (R == 2){ //Dois pontos encontrados em sequencia (nao se sabe onde quebrar o numero)
        return Cerros(132, Pl);
      }
    } else if (I == 1){ //Caractere (Id ou Palavra reservada)
      R = leId();
      if (R == 3){ //Caractere invalido
        return Cerros(128, Pl);
      } else if (R == 4){ //String muito longa
        return Cerros(129, Pl);
      }
    } else if (I == 2){ //Simbolo
      if (U == '\"'){ //Inicio de string
        fprintf(OUT, "%c", U);
        fscanf (IN, "%c", &U);
        if (U == '\"'){ //Aspas imediatamente apos outras (Ilegal)
          return Cerros(130, Pl);
        }
        while (U != '\"' && !feof(IN)){ //Imprime toda a string no arquivo de saida
          fprintf(OUT, "%c", U);
          fscanf (IN, "%c", &U);
        }
        fprintf(OUT, "%c", U); //Imprime as ultimas aspas no arquivo de saida
      }else if (U == '\n'){ //Quebra de linha
        Pl++;
        fprintf(OUT, "%c", U);
      } else if (U == '#'){ //Comentario
        while (U != '\n' && !feof(IN)){ //Enquanto nao for o fim do arquivo ou quebra de linha
          fscanf (IN, "%c", &U);
        }
        Pl++;
      } else { //Qualquer outra coisa (Simbolos reconhecidos)
        fprintf(OUT, "%c", U);
      }
    } else {
      return Cerros(128, Pl);
    }
  }

  return 0;

}

int checkToken(){ //Se for qualquer uma das palavras reservadas retorna 1
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
    //printf("-%c-\n", U);
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
    //printf("-%c-\n", U);
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
