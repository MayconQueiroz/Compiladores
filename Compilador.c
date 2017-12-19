#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "Erros.h"
#include "Checker.h"
#include "Fila.h"
#include "idtable.h"
#include "PilhasL.h"

/**
* Recebe codigo.cnm
*/

FILE *IN1; //Arquivo de entrada
FILE *OUT0; //Arquivo de strings
FILE *INM; //Arquivo de codigo intermediario

char U; //Caractere sendo lido
char L[256]; //String para atribuicao de identificadores e numeros
int Pl; //Posicao da linha (Contado com \n)
int Pla; //Auxiliar de Pl e curinga apos analise lexica
TFila* Fini; //Ponteiros para inicio e fim da fila de tokens
TFila* Ffin; //Fin = posicao a inserir
TFila* Decl; //Ponteiro para posicao onde se inserem tokens
Elemento* lista; //Lista de id's
int QSTR = 0; //Quantidade de strings no arquivo de strings
int QVAR = 0; //Quantidade de variaveis
char *VAR; //Vetor a ser alocado depois para informacoes dos ids
int deb = 0; //Debug mode
int RSTMTC = 0; //Label de saida dos lacos de repeticao (para validacao do brk)
char fltuse = 0; //Uso de floats em atribuicoes
char Eflt = 0; //Atribuicao a floats
char urel = 0; //Uso de relacionais e coisas que o float nao pode
int LGEN = 0; //Quantidade de labels usadas
Pilha* pilha; //Pilha de atribuicoes
int QVARold; //Guarda a antiga situacao do QVAR

//Declaracao de funcoes usadas a posteriori
int leId();
int leNumero();
void optok();
void checaId();
void apagaTudo();
TFila* stmt(TFila* f);

/////////////////////////////////////////////////////MAIN

int main(int argc, char *argv[]){
  int I, R, h = 0, i;
  if (argc == 2 || argc == 3){
    for (i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "/h") == 0 || strcmp(argv[i], "/H") == 0 || strcmp(argv[i], "-H") == 0  || strcmp(argv[i], "/?") == 0) {
        return Erros(1, 0);
      }
      if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "/d") == 0 || strcmp(argv[i], "/D") == 0 || strcmp(argv[i], "-D") == 0) {
        deb = 1;
        R = i;
      }
    }
    if (deb == 1){ //Debug mode on
      if (R == 1){
        if (strlen(argv[2]) < 5){
          return Erros(1, 0);
        }
      } else if (R == 2){
        if (strlen(argv[1]) < 5){
          return Erros(1, 0);
        }
      }
    } else {
      if (argc == 3){
        return Erros(1, 0);
      }
    }
	} else {
    return Erros(1, 0);
  }

  //Abertura Arquivos
  if (deb == 1){ //Debug mode on
    if (R == 1){
      IN1 = fopen(argv[2], "r");
    }
    else if (R == 2){
      IN1 = fopen(argv[1], "r");
    }
  } else {
    IN1 = fopen(argv[1], "r");
  }
  OUT0 = fopen("auxiliar.snm", "w");
  INM = fopen("CodInter.inm", "w");
  if (IN1 == NULL){
    return Erros(2, 0);
  }
  if (OUT0 == NULL){
    return Erros(3, 0);
  }
  if (INM == NULL){
    return Erros(4, 0);
  }

  //Inicializacoes
  Pl = 1;
  Fini = fila_cria();
  Ffin = fila_cria();
  lista = lista_cria();

  //Leitura Codigo Fonte
  while (!feof(IN1)){
    fscanf (IN1, "%c", &U);
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
      if (U == ' '){
        //Do nothing
      } else if (U == '\"'){ //Inicio de string
        fscanf (IN1, "%c", &U);
        if (U == '\"'){ //Aspas imediatamente apos outras (Ilegal)
          //printf("Entrei aqui pras aspas\n");
          apagaTudo();
          return Erros(130, Pl);
        }
        while (U != '\"' && !feof(IN1)){ //Imprime toda a string no arquivo de saida
          fprintf(OUT0, "%c", U);
          fscanf (IN1, "%c", &U);
        }
        fprintf(OUT0, "\n"); //Quebra linha
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
        while (U != '\n' && !feof(IN1)){ //Enquanto nao for o fim do arquivo ou quebra de linha
          fscanf (IN1, "%c", &U);
        }
        Pl++;
        Pla++;
        if (Pla == 256){ //Estourou
          return Erros(141 , Pl-Pla); //Pl-Pla para passar a linha que comeca o problema
        }
      } else if (U == '\''){ //Caractere entre aspas simples
        fscanf (IN1, "%c", &U);
        if (U == '\''){ //Aspas imediatamente apos outras (Ilegal)
          apagaTudo();
          return Erros(130, Pl);
        }
        R = impCaractere(U);
        if (R == 1){ //Se for um caractere imprimivel
          apagaTudo();
          return Erros(140, Pl);
        }
        L[0] = U;
        L[1] = '\0';
        fscanf (IN1, "%c", &U);
        if (U != '\''){ //Nao fecha com aspas simples
          apagaTudo();
          return Erros(142, Pl);
        }
        Pla = (Pla & 255) + 768;
        Ffin = fila_inserestr(Ffin, Pla, L);
        Pla = 0;
      } else { //Qualquer outra coisa (Simbolos reconhecidos)
        optok();
      }
    } else {
      apagaTudo();
      return Erros(128, Pl);
    }
    if (h == 0 && Ffin != NULL){
      Fini = Ffin;
      h = 1;
    }
  }

  if (Fini == NULL){ //Nao compila arquivos vazios
    Erros(271, Pl);
  }

  Ffin = fila_insereint(Ffin, 3328, 0);
  if (deb == 1){
    fila_imprime(Fini); //Impressao da fila (para debug)
  }

  //////////////////////////////////////////////// Fim Analise Lexica
  //Variaveis que podem ser aproveitadas:
  //int R, Pla, h, I
  //char U, L[256]
  fclose(OUT0);
  OUT0 = fopen("auxiliar.snm", "r");
  if (OUT0 == NULL){
    return Erros(5, 0);
  }
  if (deb == 1){
    fila_imprime(Fini); //Impressao da fila (para debug)
  }
  QVARold = QVAR;
  lista = lista_apagar(lista);
  pilha = pilha_cria(pilha);
  Pl = 1; //Volta pra primeira linha
  Pl = Pl + (Fini->info & 255);
  //Alocacao do vetor de Variaveis
  VAR = (char *) calloc(QVAR, sizeof(char));
  if (deb == 1){
    for (i = 0; i < QVARold; i++) {
      printf("VAR[%i] = %u. ", i, VAR[i]);
    }
    printf("\n");
  }

  Ffin = stmt(Fini); //Regras da gramatica

  if (deb == 1){
    for (i = 0; i < QVARold; i++) {
      printf("VAR[%i] = %u. ", i, VAR[i]);
    }
    printf("\n");
  }
  if (!((Ffin->info & 3840) == 3328)){
    Erros(256, Pl);
  }
  //////////////////////////////////////////////// Fim Analise Sintatica e Semantica

  return 0;
}

/////////////////////////////////////////////////////FIM DA MAIN

//Separa os tokens que interessam
void optok(){
  int i = 1;
  L[0] = U;
  if (U == '<'){ //<< ou <=
    fscanf (IN1, "%c", &U);
    if (U == '<' || U == '='){
      L[1] = U;
      i = 2;
    }
  }else if (U == '>'){ //>= ou >>
    fscanf (IN1, "%c", &U);
    if (U == '>' || U == '='){
      L[1] = U;
      i = 2;
    }else {
      fseek(IN1, -1, SEEK_CUR); //Retorna um caractere
    }
  } else if (U == '!'){ //! ou !=
    fscanf (IN1, "%c", &U);
    if (U == '='){
      L[1] = '=';
      i = 2;
    }else {
      fseek(IN1, -1, SEEK_CUR); //Retorna um caractere
    }
  }else if (U == '='){ //= ou ==
    fscanf (IN1, "%c", &U);
    if (U == '='){
      L[1] = '=';
      i = 2;
    }else {
      fseek(IN1, -1, SEEK_CUR); //Retorna um caractere
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
  else if (!strcmp(L, "+") || !strcmp(L, "-") || !strcmp(L, "*") || !strcmp(L, "/") ||  !strcmp(L, "%")){
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
    if (U == ';'){ //Delimitador de sentenca
      Pla = (Pla & 255) + 2816;
      Ffin = fila_inserestr(Ffin, Pla, L);
      Pla = 0;
    } else if (U != '\n' && U != ' ' && U != '\t'){ //Delimitadores de bloco
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
  while ((C == 0 || C == 1) && !feof(IN1)){ //Le enquanto forem caracteres ou digitos (assumindo que o primeiro foi caractere)
    L[i] = U;
    fscanf (IN1, "%c", &U);
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
  fseek(IN1, -1, SEEK_CUR); //Retorna um caractere
  return C;
}

//Le numeros decimais e inteiros
int leNumero(){
  int C = 0, i = 0, F = 0, AN = 0;
  while ((C == 0 || U == '.') && !feof(IN1)){ //Le enquanto forem digitos ou ponto (assumindo que o primeiro foi digito)
    L[i] = U;
    fscanf (IN1, "%c", &U);
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
  fseek(IN1, -1, SEEK_CUR); //Retorna um caractere
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
    printf("Comando ");
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
    printf("Op Logico ");
    return;
  }
  if (V == 2048){
    printf("Op Aritm ");
    return;
  }
  if (V == 2304){
    printf("Op Relac ");
    return;
  }
  if (V == 2560){
    printf("Atribuicao ");
    return;
  }
  if (V == 2816){
    printf("Del sentenca ");
    return;
  }
  if (V == 3072){
    printf("Del Bloco ");
    return;
  }
  if (V == 3328){
    printf("Fim Arquivo ");
    return;
  }
  if (V == 3584){
    printf("Id int Temp\t");
    return;
  }
  if (V == 3840){
    printf("Id flt Temp\t");
    return;
  }
}

TFila* mstmt(TFila* f);
TFila* decl_stmt(TFila* f);
TFila* lexp(TFila* f);
TFila* cond_stmt(TFila* f);
TFila* c(TFila* f, int LGENIF);
TFila* rept_stmt(TFila* f);
TFila* io_stmt(TFila* f);
TFila* iol(TFila* f);
TFila* atri(TFila* f);
TFila* term(TFila* f);
TFila* O(TFila* f);
TFila* R(TFila* f);
TFila* o(TFila* f);
TFila* n(TFila* f);
TFila* _atri(TFila* f);
TFila* _term(TFila* f);
TFila* _O(TFila* f);
TFila* _R(TFila* f);
TFila* _o(TFila* f);
TFila* _n(TFila* f);
TFila* checafprox(TFila* f){
  if (f->prox == NULL){
    Erros(256, Pl);
  }
  f = f->prox;
  Pl = Pl + (f->info & 255);
  Pla = (f->info & 3840);
  /*imprimeextenso(f->info);
  if (Pla == 256 || Pla == 1024 || Pla == 1280 || Pla == 3328){
    printf("%i\n", f->d.i);
  } else if (Pla == 1536){
    printf("%f\n", f->d.f);
  } else{
    printf("%s\n", f->d.str);
  }*/
  return f;
}

TFila* stmt(TFila* f){
  Pla = (f->info & 3840);
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
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
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
  } else if (Pla == 256){ //Id
    f = atri(f);
    f = mstmt(f);
  } else if (Pla == 3072 && strcmp(f->d.str, "}") == 0){
    return f;
  } else if (Pla == 3328){ //Fim de arquivo
    return f;
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
    printf ("Declaracao de variavel esperada\n");
    exit(-1);
  }
  if (Pla == 256){ //ID
    VAR[f->d.i] = A;
  } else {
    Erros(259, Pl);
  }
  f = checafprox(f);
  if (Pla == 2816){ //;
    f = checafprox(f);
  } else {
    Erros(260, Pl);
  }
  Decl = f;
  return f;
}

TFila* lexp(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = f->info & 3840;
  if (Pla == 256){ //Confirmacao se e um id
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if ((VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if (VAR[f->d.i] & 1){
      fprintf(INM, "i%i ", f->d.i);
    } else if (VAR[f->d.i] & 2){
      fprintf(INM, "f%i ", f->d.i);
    } else if (VAR[f->d.i] & 4){
      fprintf(INM, "c%i ", f->d.i);
    }
    f = checafprox(f);
    if (Pla == 2304){ //Operador relacional
      fprintf(INM, "%s ", f->d.str);
      f = checafprox(f);
      f = n(f);
    } else {
      Erros(261, Pl);
    }
  } else {
    Erros(259, Pl);
  }
  return f;
}

TFila* cond_stmt(TFila* f){
  int Ind;
  Pla = (f->info & 3840);
  if (Pla == 512 && strcmp(f->d.str, "iff") == 0){ //If
    fprintf(INM, "iff ");
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "(") == 0){
      f = checafprox(f);
      f = lexp(f);
      if (Pla == 3072 && strcmp(f->d.str, ")") == 0){
        f = checafprox(f);
        if (Pla == 3072 && strcmp(f->d.str, "{") == 0){
          Ind = LGEN;
          LGEN += 2;
          fprintf(INM, "gto L%i\ngto L%i\n:L%i\n", Ind, Ind+1, Ind);
          f = checafprox(f);
          f = stmt(f);
          if ((f->info & 3840) == 3072 && strcmp(f->d.str, "}") == 0){
            fprintf(INM, "gto L%i\n:L%i\n", Ind+2, Ind+1);
            f = checafprox(f);
            f = c(f, Ind+2);
            return f;
          } else {
            Erros(265, Pl);
          }
        } else {
          Erros(264, Pl);
        }
      } else {
        Erros(263, Pl);
      }
    } else {
      Erros(262, Pl);
    }
  } else if (Pla == 512 && strcmp(f->d.str, "brk") == 0){ //Break
    if (RSTMTC == 0){ //brk fora de um laco de repeticao
      Erros(513, Pl);
    }
    fprintf(INM, "gto L%i\n", RSTMTC);
    f = checafprox(f);
    if (Pla == 2816){ //;
      f = checafprox(f);
      return f;
    } else {
      Erros(260, Pl);
    }
  } else {
    printf("Esperada chamada de funcao de condicao - Possivel Linha %i\n", Pl);
    exit (-1);
  }
  return NULL;
}

TFila* c(TFila* f, int LGENIF){
  int Ind;
  Pla = (f->info & 3840);
  if (Pla == 512 && strcmp(f->d.str, "els") == 0){ //els
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "{") == 0){
      f = checafprox(f);
      f = stmt(f);
      if ((f->info & 3840) == 3072 && strcmp(f->d.str, "}") == 0){
        f = checafprox(f); //TODO tratar fim de arquivo
        fprintf(INM, ":L%i\n", LGENIF);
        return f;
      } else {
        Erros(265, Pl);
      }
    } else {
      Erros(264, Pl);
    }
  } else if (Pla == 512 || Pla == 256 || Pla == 3328 || (Pla == 3072 && strcmp(f->d.str, "}") == 0)){
    fprintf(INM, ":L%i\n", LGENIF);
    return f;
  } else {
    Erros(266, Pl);
  }
}

TFila* rept_stmt(TFila* f){
  int Ind, Tem;
  TFila *g;
  if (f == NULL){
    Erros(256, Pl);
  }
  if ((f->info & 3840) == 512 && strcmp(f->d.str, "for") == 0){
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "(") == 0){
      f = checafprox(f);
      f = atri(f); //Atribuicao inicial
      g = f; //Salvamento temporario do ponteiro para mais tarde
      f = _atri(f); //Atribuicao pra passar
      Ind = LGEN;
      LGEN += 3;
      fprintf(INM, ":L%i\niff ", Ind);
      f = lexp(f);
      Tem = RSTMTC;
      RSTMTC = Ind + 1;
      fprintf(INM, "gto L%i\ngto L%i\n:L%i\n", Ind+1, Ind+2, Ind+1);
      if ((f->info & 3840) == 3072 && strcmp(f->d.str, ")") == 0){
        f = checafprox(f);
        if (Pla == 3072 && strcmp(f->d.str, "{") == 0){
          f = checafprox(f);
          f = stmt(f);
          if ((f->info & 3840) == 3072 && strcmp(f->d.str, "}") == 0){
            g = atri(g);
            fprintf(INM, "gto L%i\n:L%i\n", Ind, Ind+2);
            f = checafprox(f); //TODO Tratar fim de arquivo
            RSTMTC = Tem;
            return f;
          } else {
            Erros(265, Pl);
          }
        } else {
          Erros(264, Pl);
        }
      } else {
        Erros(263, Pl);
      }
    } else {
      Erros(262, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "whl") == 0){
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "(") == 0){
      f = checafprox(f);
      Ind = LGEN;
      LGEN += 3;
      fprintf(INM, ":L%i\niff ", Ind);
      f = lexp(f);
      Tem = RSTMTC;
      RSTMTC = Ind + 1;
      fprintf(INM, "gto L%i\ngto L%i\n:L%i\n", Ind+1, Ind+2, Ind+1);
      if ((f->info & 3840) == 3072 && strcmp(f->d.str, ")") == 0){
        f = checafprox(f);
        if (Pla == 3072 && strcmp(f->d.str, "{") == 0){
          f = checafprox(f);
          f = stmt(f);
          if ((f->info & 3840) == 3072 && strcmp(f->d.str, "}") == 0){
            f = checafprox(f); //TODO Tratar fim de arquivo
            fprintf(INM, "gto L%i\n:L%i\n", Ind, Ind+2);
            RSTMTC = Tem;
            return f;
          } else {
            Erros(265, Pl);
          }
        } else {
          Erros(264, Pl);
        }
      } else {
        Erros(263, Pl);
      }
    } else {
      Erros(262, Pl);
    }
  } else {
    printf("Esperada chamada de funcao de repeticao - Possivel Linha %i\n", Pl);
    exit(-1);
  }
}

TFila* io_stmt(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  if ((f->info & 3840) == 512 && strcmp(f->d.str, "scn") == 0){
    fprintf(INM, "scn ");
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "(") == 0){
      f = checafprox(f);
      if (Pla == 256){ //Id
        if (VAR[f->d.i] == 0){ //id ja foi declarado
          Erros(512, Pl);
        }
        if (VAR[f->d.i] & 1){
          fprintf(INM, "i%i", f->d.i);
        } else if (VAR[f->d.i] & 2){
          fprintf(INM, "f%i", f->d.i);
        } else if (VAR[f->d.i] & 4){
          fprintf(INM, "c%i", f->d.i);
        }
        VAR[f->d.i] = VAR[f->d.i] | 8;
        f = checafprox(f);
        if (Pla == 3072 && strcmp(f->d.str, ")") == 0){
          f = checafprox(f);
          if ((Pla) == 2816){ //;
            fprintf(INM, "\n");
            f = checafprox(f);
            return f;
          } else {
            Erros(260, Pl);
          }
        } else {
          Erros(263, Pl);
        }
      } else {
        Erros(259, Pl);
      }
    } else {
      Erros(262, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "prt") == 0){
    fprintf(INM, "prt ");
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "(") == 0){
      f = checafprox(f);
      f = iol(f);
      if ((f->info & 3840) == 3072 && strcmp(f->d.str, ")") == 0){
        f = checafprox(f);
        if (Pla == 2816){ //;
          f = checafprox(f);
          return f;
        } else {
          Erros(260, Pl);
        }
      } else {
        Erros(263, Pl);
      }
    } else {
      Erros(262, Pl);
    }
  } else {
    printf("Esperado chamada de entrada/saida - Possivel Linha %i\n", Pl);
    exit(1);
  }
}

TFila* iol(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1024 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256){
      if (VAR[f->d.i] & 1){ //Id
        fprintf(INM, "i%i\n", f->d.i);
      } else if (VAR[f->d.i] & 2){
        fprintf(INM, "f%i\n", f->d.i);
      } else if (VAR[f->d.i] & 4){
        fprintf(INM, "c%i\n", f->d.i);
      }
    } else if (Pla == 768) { //Caractere
      fprintf(INM, "\'%s\'\n", f->d.str);
    } else if (Pla == 1024) { //String
      fprintf(INM, "\"");
      fscanf(OUT0, "%c", &U);
      while (U != '\n'){
        fprintf(INM, "%c", U);
        fscanf(OUT0, "%c", &U);
      }
      fprintf(INM, "\"\n");
    } else if (Pla == 1280) { //Inteiro
      fprintf(INM, "%i\n", f->d.i);
    } else if (Pla == 1536) { //Decimal
      fprintf(INM, "%f\n", f->d.f);
    }
    return checafprox(f);
  } else {
    Erros(267, Pl);
  }
}

TFila* atri(TFila* f){
  TFila* g;
  if (f == NULL){
    Erros(256, Pl);
  }
  Eflt = 0;
  fltuse = 0;
  urel = 0;
  g = f; //Guardando ponteiro
  if ((f->info & 3840) == 256){ //Id
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if ((VAR[f->d.i] & 2) != 0){ //Marcacao que espera um float
      Eflt = 1;
    }
    VAR[f->d.i] = VAR[f->d.i] | 8;
    f = checafprox(f);
    if (Pla == 2560){ //=
      f = checafprox(f);
      f = term(f);
      if (VAR[g->d.i] & 1){ //Id
        fprintf(INM, "i%i = i%i\n", g->d.i, QVAR-1);
      } else if (VAR[g->d.i] & 2){
        fprintf(INM, "f%i = f%i\n", g->d.i, QVAR-1);
      } else if (VAR[g->d.i] & 4){
        fprintf(INM, "c%i = i%i\n", g->d.i, QVAR-1);
      }
      if ((f->info & 3840) == 2816){ //;
        f = checafprox(f);
        if (urel != 0 && fltuse != 0){ //Float problem
          Erros(514, Pl-1);
        }
        if (fltuse != 0 && Eflt == 0){ //Uso de float e atribuicao a nao float
          Erros(515, Pl-1);
        }
        return f;
      } else {
        Erros(260, Pl);
      }
    } else {
      Erros(268, Pl);
    }
  } else {
    printf("Esperada atribuicao - Possivel linha %i", Pl);
    exit(1);
  }
}

TFila* term(TFila* f){
  TFila* g;
  if (f == NULL){
    Erros(256, Pl);
  }
  g = f->prox;
  if (g == NULL || g->info == 3328){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    } else if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    if (Pla == 256 ||  Pla == 1280){ //Insercoes na pilha de atribuicoes
      pilha = pilha_insereint(pilha, f->info, f->d.i);
    } else if (Pla == 1536){
      pilha = pilha_insereflt(pilha, f->info, f->d.f);
    } else if (Pla == 768){
      pilha = pilha_inserestr(pilha, f->info, f->d.str);
    }
    if (g->info == 2816){
      if (fltuse != 0){
        fprintf(INM, "f%i = ", QVAR);
      } else{
        fprintf(INM, "i%i = ", QVAR);
      }
      QVAR++;
      f = n(f);
      fprintf(INM, "\n");
    } else{
      f = _n(f);
    }
    f = O(f);
    f = R(f);
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* O(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if ((Pla == 1792) && strcmp(f->d.str, "!") == 0){
    Pla = (pilha->info & 3840);
    if (Pla == 256 && (VAR[pilha->d.i] & 1) != 0){ //Insercoes na pilha de atribuicoes
      fprintf(INM, "i%i = ! i%i\n", QVAR, pilha->d.i);
    } else if (Pla == 256 && (VAR[pilha->d.i] & 2) != 0){ //Insercoes na pilha de atribuicoes
      fprintf(INM, "i%i = ! f%i\n", QVAR, pilha->d.i);
    } else if (Pla == 256 && (VAR[pilha->d.i] & 4) != 0){ //Insercoes na pilha de atribuicoes
      fprintf(INM, "i%i = ! c%i\n", QVAR, pilha->d.i);
    } else if (Pla == 1280){
      fprintf(INM, "i%i = ! %i\n", QVAR, pilha->d.i);
    } else if (Pla == 1536 || Pla == 3840){
      Erros(514, Pl);
    } else if (Pla == 768){
      fprintf(INM, "i%i = ! \'%s\'\n", QVAR, pilha->d.str);
    }
    pilha = pilha_remove(pilha);
    pilha = pilha_insereint(pilha, 256, QVAR);
    QVAR++;
    urel = 1;
    return checafprox(f);
  } else if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536 || Pla == 2816 || Pla == 1792 || Pla == 2048){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* R(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    f = term(f);
    f = o(f);
    f = O(f);
    f = R(f);
    return f;
  } else if (Pla == 2816 || Pla == 1792 || Pla == 2048){
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* o(TFila* f){
  Pilha* a;
  int seg;
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 1792 || Pla == 2048){
    if (Pla == 1792 || (Pla == 2048 && strcmp(f->d.str, "%") == 0)){
      urel = 1;
    }
    if (Pla == 1792 && strcpy(f->d.str, "!") == 0){
      Pla = (pilha->info & 3840);
      if (Pla == 256){ //Insercoes na pilha de atribuicoes
        fprintf(INM, "i%i = ! i%i\n", QVAR, pilha->d.i);
      } else if (Pla == 1280){
        fprintf(INM, "i%i = ! %i\n", QVAR, pilha->d.i);
      } else if (Pla == 1536 || Pla == 3840){
        Erros(514, Pl);
      } else if (Pla == 768){
        fprintf(INM, "i%i = ! \'%s\'\n", QVAR, pilha->d.str);
      }
      pilha = pilha_remove(pilha);
      pilha = pilha_insereint(pilha, 256, QVAR);
      QVAR++;
    } else { ///////////////////////////////////////////////Reducao da pilha
      Pla = (pilha->info & 3840);
      a = pilha->prox;
      QSTR = (a->info & 3840);
      if (Pla == 1536 || Pla == 3840 || (Pla == 256 && (VAR[pilha->d.i] & 2) != 0) || QSTR == 1536 || QSTR == 3840 || (QSTR == 256 && (VAR[a->d.i] & 2) != 0)){ //Coercao para float
        fprintf(INM, "f%i = ", QVAR);
        seg = 3840;
      } else {
        fprintf(INM, "i%i = ", QVAR);
        seg = 256;
      }
      if (Pla == 256 && (VAR[pilha->d.i] & 1) != 0){ //Id int
        fprintf(INM, "i%i ", pilha->d.i);
      } else if ((Pla == 256 && (VAR[pilha->d.i] & 2) != 0) || Pla == 3840) { //Id flt
        fprintf(INM, "f%i ", pilha->d.i);
      } else if (Pla == 256 && (VAR[pilha->d.i] & 4) != 0) { //Id chr
        fprintf(INM, "c%i ", pilha->d.i);
      } else if (Pla == 768) { //chr
        fprintf(INM, "\'%s\' ", pilha->d.str);
      } else if (Pla == 1280) { //int
        fprintf(INM, "%i ", pilha->d.i);
      } else if (Pla == 1536) { //flt
        fprintf(INM, "%f ", pilha->d.f);
      }
      fprintf(INM, "%s ", f->d.str); //Operador
      pilha = pilha_remove(pilha);
      if (Pla == 256 && (VAR[pilha->d.i] & 1) != 0){ //Id int
        fprintf(INM, "i%i\n", pilha->d.i);
      } else if ((Pla == 256 && (VAR[pilha->d.i] & 2) != 0) || Pla == 3840) { //Id flt
        fprintf(INM, "f%i\n", pilha->d.i);
      } else if (Pla == 256 && (VAR[pilha->d.i] & 4) != 0) { //Id chr
        fprintf(INM, "c%i\n", pilha->d.i);
      } else if (Pla == 768) { //chr
        fprintf(INM, "\'%s\'\n", pilha->d.str);
      } else if (Pla == 1280) { //int
        fprintf(INM, "%i\n", pilha->d.i);
      } else if (Pla == 1536) { //flt
        fprintf(INM, "%f\n", pilha->d.f);
      }
      pilha = pilha_remove(pilha);
      pilha = pilha_insereint(pilha, seg, QVAR);
      QVAR++;
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}

TFila* n(TFila* f){
  int i;
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    if (Pla == 256){ //Insercoes na pilha de atribuicoes
      if ((VAR[pilha->d.i] & 1) != 0 || Pla == 3584){ //Id int
        fprintf(INM, "i%i ", pilha->d.i);
      } else if ((VAR[pilha->d.i] & 2) != 0 || Pla == 3840) { //Id flt
        fprintf(INM, "f%i ", pilha->d.i);
      } else if ((VAR[pilha->d.i] & 4) != 0) { //Id chr
        fprintf(INM, "c%i ", pilha->d.i);
      }
      pilha = pilha_insereint(pilha, f->info, f->d.i);
    } else if (Pla == 1280){
      fprintf(INM, "%i ", f->d.i);
      pilha = pilha_insereint(pilha, f->info, f->d.i);
    } else if (Pla == 1536){
      fprintf(INM, "%f ", f->d.f);
      pilha = pilha_insereflt(pilha, f->info, f->d.f);
    } else if (Pla == 768){
      fprintf(INM, "\'%s\' ", f->d.str);
      pilha = pilha_inserestr(pilha, f->info, f->d.str);
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}

///////////////////////////////////////Funcoes subtraco (nao imprimem no arquivo de saida)

TFila* _atri(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Eflt = 0;
  fltuse = 0;
  urel = 0;
  if ((f->info & 3840) == 256){ //Id
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if ((VAR[f->d.i] & 2) != 0){ //Marcacao que espera um float
      Eflt = 1;
    }
    VAR[f->d.i] = VAR[f->d.i] | 8;
    f = checafprox(f);
    if (Pla == 2560){ //=
      f = checafprox(f);
      f = _term(f);
      if ((f->info & 3840) == 2816){ //;
        f = checafprox(f);
        if (urel != 0 && fltuse != 0){ //Float problem
          Erros(514, Pl-1);
        }
        if (fltuse != 0 && Eflt == 0){ //Uso de float e atribuicao a nao float
          Erros(515, Pl-1);
        }
        return f;
      } else {
        Erros(260, Pl);
      }
    } else {
      Erros(268, Pl);
    }
  } else {
    printf("Esperada atribuicao - Possivel linha %i", Pl);
    exit(1);
  }
}

TFila* _term(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    f = _n(f);
    f = _O(f);
    f = _R(f);
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* _O(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if ((Pla == 1792) && strcmp(f->d.str, "!") == 0){
    urel = 1;
    return checafprox(f);
  } else if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536 || Pla == 2816 || Pla == 1792 || Pla == 2048){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* _R(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    f = _term(f);
    f = _o(f);
    f = _O(f);
    f = _R(f);
    return f;
  } else if (Pla == 2816 || Pla == 1792 || Pla == 2048){
    return f;
  } else {
    Erros(269, Pl);
  }
}

TFila* _o(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 1792 || Pla == 2048){
    if (Pla == 1792 || (Pla == 2048 && strcmp(f->d.str, "%") == 0)){
      urel = 1;
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}

TFila* _n(TFila* f){
  int i;
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256 || Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 256 && VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (Pla == 256 && (VAR[f->d.i] & 8) == 0){ //Variavel nao foi inicializada
      Erros(516, Pl);
    }
    if ((Pla == 256 && VAR[f->d.i] == 2) || Pla == 1536){ //Uso de float
      fltuse = 1;
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}
