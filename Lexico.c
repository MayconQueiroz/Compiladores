#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"
#include "Checker.h"
#include "Fila.h"
#include "idtable.h"

/**
* Recebe codigo.cnm e escreve em arquivo codigo.lnm
*/

FILE *IN; //Arquivo de entrada
FILE *OUT; //Arquivo de saida

char U; //Caractere sendo lido
char L[256]; //String para atribuicao de identificadores e numeros
int Pl; //Posicao da linha (Contado com \n)
int Pla; //Auxiliar de Pl e curinga apos analise lexica
TFila* Fini; //Ponteiros para inicio e fim da fila de tokens
TFila* Ffin; //Fin = posicao a inserir
Elemento* lista; //Lista de id's
int QSTR = 0; //Quantidade de strings no arquivo de strings
int QVAR = 0; //Quantidade de variaveis
char *VAR;

//Declaracao de funcoes usadas a posteriori
int leId();
int leNumero();
void optok();
void checaId();
void apagaTudo();
TFila* stmt(TFila* f);

/////////////////////////////////////////////////////MAIN

int main(int argc, char *argv[]){
  int I, R, h = 0;
  if (argc != 2){
		return Erros(1, 0);
	}

  //Abertura Arquivos
  IN = fopen(argv[1], "r");
  OUT = fopen("auxiliar.snm", "w");
  if (IN == NULL){
    return Erros(2, 0);
  }
  if (OUT == NULL){
    return Erros(3, 0);
  }

  //Inicializacoes
  Pl = 1;
  Fini = fila_cria();
  Ffin = fila_cria();
  lista = lista_cria();

  //Leitura Codigo Fonte
  while (!feof(IN)){
    fscanf (IN, "%c", &U);
    I = QueEso(U); //Funcao que retorna codigo de caractere (vide Checker.h)
    if (I == 0){ //Numero (Inteiro ou decimal)
      R = leNumero();
      if (R != 0){ //1 - Numero muito extenso; 2 - Dois pontos encontrados em sequencia (nao se sabe onde quebrar o numero); 3 - Ausencia de numeros apos o ponto
        apagaTudo();
        return Erros(R + 130, Pl);
      }
    } else if (I == 1){ //Caractere (Id ou Palavra reservada)
      R = leId();
      if (R == 3){ //Caractere invalido
        apagaTudo();
        return Erros(128, Pl);
      } else if (R == 4){ //String muito longa
        apagaTudo();
        return Erros(129, Pl);
      }
    } else if (I == 2){ //Simbolo
      if (U == '\"'){ //Inicio de string
        fscanf (IN, "%c", &U);
        if (U == '\"'){ //Aspas imediatamente apos outras (Ilegal)
          apagaTudo();
          return Erros(130, Pl);
        }
        while (U != '\"' && !feof(IN)){ //Imprime toda a string no arquivo de saida
          fprintf(OUT, "%c", U);
          fscanf (IN, "%c", &U);
        }
        fprintf(OUT, "\n"); //Quebra linha
        Pla = (Pla & 255) + 1024;
        Ffin = fila_insereint(Ffin, Pla, QSTR);
        Pla = 0;
        QSTR++;
      } else if (U == '\n'){ //Quebra de linha
        Pl++;
        Pla++;
        if (Pla == 256){ //Estourou
          return Erros(141 , Pl-Pla); //Pl-Pla para passar a linha que comeca o problema
        }
      } else if (U == '#'){ //Comentario
        while (U != '\n' && !feof(IN)){ //Enquanto nao for o fim do arquivo ou quebra de linha
          fscanf (IN, "%c", &U);
        }
        Pl++;
        Pla++;
        if (Pla == 256){ //Estourou
          return Erros(141 , Pl-Pla); //Pl-Pla para passar a linha que comeca o problema
        }
      } else if (U == '\''){ //Caractere entre aspas simples
        fscanf (IN, "%c", &U);
        R = impCaractere(U);
        if (R == 1){ //Se for um caractere imprimivel
          apagaTudo();
          return Erros(140, Pl);
        }
        L[0] = U;
        L[1] = '\0';
        fscanf (IN, "%c", &U);
        if (U != '\''){ //Nao fecha com aspas simpels
          apagaTudo();
          return Erros(130, Pl);
        }
        Pla = (Pla & 255) + 768;
        Ffin = fila_inserestr(Ffin, Pla, L);
        Pla = 0;
      } else { //Qualquer outra coisa (Simbolos reconhecidos)
        optok();
      }
    } else {
      apagaTudo();
      printf ("\"%c\" ", U);
      return Erros(128, Pl);
    }
    if (h == 0 && Ffin != NULL){
      Fini = Ffin;
      h = 1;
    }
  }
  //////////////////////////////////////////////// Fim Analise Lexica
  //Variaveis que podem ser aproveitadas:
  //int R, Pla, h, I
  //char U, L[256]
  //fila_imprime(Fini); //Impressao da fila (para debug)
  lista = lista_apagar(lista);
  Pl = 1; //Volta pra primeira linha
  Pl = Pl + (Fini->info & 255);
  //Alocacao do vetor de Variaveis
  VAR = (char *) calloc(QVAR, sizeof(char));

  Ffin = stmt(Fini);
  //////////////////////////////////////////////// Fim Analise Semantica
  return 0;
}

/////////////////////////////////////////////////////FIM DA MAIN

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
    Pla = (Pla & 255) + 1792;
    Ffin = fila_inserestr(Ffin, Pla, L);
    Pla = 0;
  }
  //Operadores Aritmeticos
  else if (!strcmp(L, "+") || !strcmp(L, "-") || !strcmp(L, "*") || !strcmp(L, "/") ||  !strcmp(L, "%%")){
    Pla = (Pla & 255) + 2048;
    Ffin = fila_inserestr(Ffin, Pla, L);
    Pla = 0;
  }
  //Operadores Relacionais
  else if (!strcmp(L, "==") || !strcmp(L, "!=") || !strcmp(L, "<=") || !strcmp(L, ">=") || !strcmp(L, "<") || !strcmp(L, ">")){
    Pla = (Pla & 255) + 2304;
    Ffin = fila_inserestr(Ffin, Pla, L);
    Pla = 0;
  }
  //Atribuicao
  else if (!strcmp(L, "=")){
    Pla = (Pla & 255) + 2560;
    Ffin = fila_inserestr(Ffin, Pla, L);
    Pla = 0;
  }
  else {
    if (U == ';'){
      Pla = (Pla & 255) + 2816;
      Ffin = fila_inserestr(Ffin, Pla, L);
      Pla = 0;
    } else if (U != '\n' && U != ' ' && U != '\t'){
      Pla = (Pla & 255) + 3072;
      Ffin = fila_inserestr(Ffin, Pla, L);
      Pla = 0;
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
      Pla = (Pla & 255) + 512;
      Ffin = fila_inserestr(Ffin, Pla, L);
      Pla = 0;
    } else {
      checaId();
    }
  } else {
    checaId();
  }
  fseek(IN, -1, SEEK_CUR); //Retorna um caractere
  return C;
}

//Le numeros decimais e inteiros
int leNumero(){
  int C = 0, i = 0, F = 0, AN = 0;
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
    if (L[i-1] == '.'){ //Sem numeros apos o ponto
      return 3;
    }
    C = NumeroCorecto(L, 0);
    if (C == 1){
      return 4;
    }
    Pla = (Pla & 255) + 1536;
    Ffin = fila_insereflt(Ffin, Pla, atof(L));
    Pla = 0;
  } else { //Inteiro
    C = NumeroCorecto(L, 1);
    if (C == 1){
      return 4;
    }
    Pla = (Pla & 255) + 1280;
    Ffin = fila_insereint(Ffin, Pla, atoi(L));
    Pla = 0;
  }
  fseek(IN, -1, SEEK_CUR); //Retorna um caractere
  return 0;
}

//Faz as operacoes de tabelamento dos id's
void checaId(){
  int E;
  E = lista_procura(lista, L);
  if (E == -1){
    lista = lista_insere(lista, L);
    E = lista_procura(lista, L);
    Pla = (Pla & 255) + 256;
    Ffin = fila_insereint(Ffin, Pla, E);
    Pla = 0;
    QVAR += 1;
  } else {
    Pla = (Pla & 255) + 256;
    Ffin = fila_insereint(Ffin, Pla, E);
    Pla = 0;
  }
}

//Libera todas as posicoes alocadas para evitar lixo de memoria
void apagaTudo(){
  lista = lista_apagar(lista);
  Fini = fila_apagar(Fini);
  Ffin = NULL;
}

///////////////////////////////////////////////////INICIO DAS FUNCOES SINTATICAS

void imprimeextenso (int V){
  if (V == 256){
    printf("Variavel ");
    return;
  }
  if (V == 512){
    printf("Comando interno ");
    return;
  }
  if (V == 768){
    printf("Caractere ");
    return;
  }
  if (V == 1024){
    printf("String ");
    return;
  }
  if (V == 1280){
    printf("Inteiro ");
    return;
  }
  if (V == 1536){
    printf("Decimal ");
    return;
  }
  if (V == 1792){
    printf("Operador Logico ");
    return;
  }
  if (V == 2048){
    printf("Operador Aritmetico ");
    return;
  }
  if (V == 2304){
    printf("Operador Relacional ");
    return;
  }
  if (V == 2560){
    printf("Atribuicao ");
    return;
  }
  if (V == 2816){
    printf("Ponto e Virgula ");
    return;
  }
  if (V == 3072){
    printf("Delimitador de Bloco ");
    return;
  }
}

TFila* mstmt(TFila* f);
TFila* decl_stmt(TFila* f);
TFila* lexp(TFila* f);
TFila* cond_stmt(TFila* f);
TFila* checafprox(TFila* f){
  if (f->prox == NULL){
    Erros(256, Pl);
  }
  f = f->prox;
  Pl = Pl + (f->info & 255);
  return f;
}

TFila* stmt(TFila* f){
  Pla = f->info & 3840;
  if (Pla == 512){ //Palavra reservada
    if (strcmp(f->d.str, "int") == 0 || strcmp(f->d.str, "flt") == 0 || strcmp(f->d.str, "chr") == 0){
      f = decl_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "iff") == 0 || strcmp(f->d.str, "brk") == 0){
      f = cond_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "for") == 0 || strcmp(f->d.str, "whl") == 0){
      f = rept_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "prt") == 0 || strcmp(f->d.str, "scn") == 0){
      f = io_stmt(f);
      f = mstmt(f);
    } else {
      Erros(257, Pl);
    }
  } else if (Pla == 256){
    f = atri(f);
    f = mstmt(f);
  } else {
    imprimeextenso(Pla);
    Erros(258, Pl);
  }
  return f;
}

TFila* mstmt(TFila* f){
  if (f == NULL){
    return NULL;
  }
  Pla = f->info & 3840;
  if (Pla == 512){ //Palavra reservada
    if (strcmp(f->d.str, "int") == 0 || strcmp(f->d.str, "flt") == 0 || strcmp(f->d.str, "chr") == 0){
      f = decl_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "iff") == 0 || strcmp(f->d.str, "brk") == 0){
      f = cond_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "for") == 0 || strcmp(f->d.str, "whl") == 0){
      f = rept_stmt(f);
      f = mstmt(f);
    } else if (strcmp(f->d.str, "prt") == 0 || strcmp(f->d.str, "scn") == 0){
      f = io_stmt(f);
      f = mstmt(f);
    } else {
      Erros(257, Pl);
    }
  } else if (Pla == 256){
    f = atri(f);
    f = mstmt(f);
  } else if (Pla == 3072 && strcmp(f->d.str, "}") == 0){
    f = f->prox;
    if (f != NULL){
      Pl = Pl + (f->info & 255);
    }
  } else {
    imprimeextenso(Pla);
    Erros(258, Pl);
  }
  return f;
}

TFila* decl_stmt(TFila* f){
  char A = 0;
  if (strcmp(f->d.str, "int") == 0){
    f = checafprox(f);
    A = 1;
  } else if (strcmp(f->d.str, "flt") == 0){
    f = checafprox(f);
    A = 2;
  } else if (strcmp(f->d.str, "chr") == 0){
    f = checafprox(f);
    A = 4;
  } else {
    printf ("Erro com a chamada da funcao decl_stmt\n");
    exit(-1);
  }
  if ((f->info & 3840) == 256){ //O proximo e um ID
    VAR[f->d.i] = A;
  } else {
    Erros(259, Pl);
  }
  f = checafprox(f);
  if ((f->info & 3840) == 2816){ //O proximo e um delimitador de sentenca
    f = checafprox(f);
  } else {
    Erros(260, Pl);
  }
  return f;
}

TFila* lexp(TFila* f){
  Pla = f->info & 3840;
  if (Pla == 256){ //Confirmacao se e um id
    f = checafprox(f);
    if ((f->info & 3840) == 2304){ //Operador relacional
      f = checafprox(f);
      f = qc(f);
    } else {
      Erros(261, Pl);
    }
  } else {
    Erros(259, Pl);
  }
  return f;
}

TFila* cond_stmt(TFila* f){
 return NULL;
}
