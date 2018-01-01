#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"
#include "Checker.h"
#include "Fila.h"
#include "idtable.h"
#include "PilhasL.h"

FILE *IN1; //Arquivo de entrada
TFila* Fini; //Ponteiros para inicio e fim da fila de tokens
TFila* Ffin; //Fin = posicao a inserir
Elemento* lista; //Lista de id's
Pilha* pilha; //Pilha de atribuicoes
char *VAR; //Vetor a ser alocado depois para informacoes dos ids
int *VVAR; //Tamanho dos vetores VAR
char U; //Caractere sendo lido
char L[256]; //String para atribuicao de identificadores e numeros
int Pl; //Posicao da linha (Contado com \n)
int Pla; //Auxiliar de Pl e curinga apos analise lexica
int QVAR = 0; //Quantidade de variaveis
char fltuse = 0; //Uso de floats em atribuicoes
char Eflt = 0; //Atribuicao a floats
char urel = 0; //Uso de relacionais e coisas que o float nao pode
int varpos = 0; //Posicao do var para atribuicao do tamanho do vetor
int Pla = 0;

void help(){
  printf("Demonstra utilizacao da pilha em atribuicoes em notacao polonesa reversa para a linugagem NOME\n\n");
  printf("Atri arquivo\n\n  arquivo\tEspecifica o arquivo de entrada com apenas uma atribuicao dentro do arquivo");
  exit(1);
}
int leId();
int leNumero();
void optok();
void checaId();
void apagaTudo();
TFila* stmt(TFila* f);

int main(int argc, char const *argv[]) {
  int I, R, h = 0, i;
  if (argc != 2){
    help();
  }
  IN1 = fopen(argv[1], "r");
  if (IN1 == NULL){
    printf("Erro ao abrir o arquivo\n");
    return 1;
  }
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
      } else if (U == '\"'){ //Inicio de string
        printf("Nao sao permitdas strings em arquivos deste tipo\n");
        return -1;
      } else if (U == '#'){ //Comentario
        printf("Nao sao permitdos copmentarios em arquivos deste tipo\n");
        return -1;
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
  fila_imprime(Fini);
  lista = lista_apagar(lista);
  pilha = pilha_cria(pilha);
  Ffin = fila_insereint(Ffin, 3328, 0);
  VAR = (char *) calloc(QVAR, sizeof(char));
  if (VAR == NULL){ //Falha de alocacao
    return Erros(21, 0);
  }
  VVAR = (int *) calloc(QVAR, sizeof(int));
  if (VVAR == NULL){ //Falha de alocacao
    return Erros(21, 0);
  }
  pilha = pilha_cria();
  Ffin = stmt(Fini); //Regras da gramatica
  return 0;
}

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
    printf("Nao sao permitidos operadores relacionais para arquivos deste tipo\n");
    exit(-1);
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
  if (strcmp(L, "chr") == 0 || strcmp(L, "int") == 0 || strcmp(L, "flt") == 0){
    return 1;
  }
  if (strcmp(L, "brk") == 0 || strcmp(L, "scn") == 0 || strcmp(L, "iff") == 0 || strcmp(L, "els") == 0 || strcmp(L, "for") == 0 || strcmp(L, "whl") == 0 || strcmp(L, "prt") == 0){
    printf("Somente declaracoes sao permitidas para arquivos deste tipo\n");
    exit(-1);
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
  if (C == 3){ //Caractere Invalido
    return 3;
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
  if (C == 3){ //Caractere Invalido
    Erros(140, Pl);
    exit(-1);
  }
  L[i] = '\0'; //Termina numero
  if (F == 1){ //Decimal
    if (L[i-1] == '.'){ //Sem numeros apos o ponto
      return 3;
    }
    C = NumeroCorecto(L, 0);
    if (C == 1){ //Numero nao representavel
      return 4;
    } else if (C == 2){ //Bug na chamada
      return 5;
    }
    Pla = (Pla & 255) + 1536;
    Ffin = fila_insereflt(Ffin, Pla, atof(L));
    Pla = 0;
  } else { //Inteiro
    C = NumeroCorecto(L, 1);
    if (C == 1){ //Numero nao representavel
      return 4;
    } else if (C == 2){ //Bug na chamada
      return 5;
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

///////////////////////////////////////////////////Gramatica a parte
TFila* decl_stmt(TFila* f);
TFila* d(TFila* f);
TFila* atri(TFila* f);
TFila* term(TFila* f);
TFila* O(TFila* f);
TFila* R(TFila* f);
TFila* o(TFila* f);
TFila* n(TFila* f);
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
      f = stmt(f);
    } else {
      printf("Somente declaracoes e uma expressao aritmetica\n");
      exit(-1);
    }
  } else if (Pla == 256){
    f = atri(f);
  } else {
    printf("Somente declaracoes e uma expressao aritmetica\n");
    exit(-1);
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
    varpos = f->d.i;
    VVAR[f->d.i] = 1;
  } else {
    Erros(259, Pl);
  }
  f = checafprox(f);
  f = d(f);
  if (Pla == 2816){ //;
    f = checafprox(f);
  } else {
    Erros(260, Pl);
  }
  return f;
}

TFila* d(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 3072 && strcmp(f->d.str, "[") == 0){ //Colchetes
    f = checafprox(f);
    if (Pla == 1280){ //Inteiro
      if (f->d.i <= 0){
        Erros(523, Pl);
      }
      VVAR[varpos] = f->d.i;
      f = checafprox(f);
      if (Pla == 3072 && strcmp(f->d.str, "]") == 0) { //Colchetes
        VAR[varpos] = VAR[varpos] | 16;
        return checafprox(f);
      } else {
        Erros(276, Pl);
      }
    } else {
      Erros(275, Pl);
    }
  } else { //La ele trata o erro
    return f;
  }
}

void imprimemesmo(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 256){ //id
    if (VAR[f->d.i] & 1){ //Id
      printf("i%i", f->d.i);
    } else if (VAR[f->d.i] & 2){
      printf("f%i", f->d.i);
    } else if (VAR[f->d.i] & 4){
      printf("c%i", f->d.i);
    } else {
      printf("Variavel usada na expressao nao foi previamente declarada\n");
    }
    if (VAR[f->d.i] & 16){ //Vetor
      f = checafprox(f);
      if (Pla == 3072 && strcmp(f->d.str, "[") == 0){ //Colchetes
        f = checafprox(f);
        printf("[");
        if (Pla == 1280){ //Inteiro ou
          printf("%i", f->d.i);
          f = checafprox(f);
          if (Pla == 3072 && strcmp(f->d.str, "]") == 0) { //Colchetes
            VAR[varpos] = VAR[varpos] | 16;
            printf("]");
            return;
          } else {
            Erros(276, Pl);
          }
        } else if (Pla == 256 && (VAR[f->d.i] & 5)){ //Id int ou chr
          if (VAR[f->d.i] & 1){ //Id
            printf("i%i", f->d.i);
          } else if (VAR[f->d.i] & 4){
            printf("c%i", f->d.i);
          }
          f = checafprox(f);
          if (Pla == 3072 && strcmp(f->d.str, "]") == 0) { //Colchetes
            VAR[varpos] = VAR[varpos] | 16;
            printf("]");
            return;
          } else {
            Erros(276, Pl);
          }
        } else {
          Erros(275, Pl);
        }
      } else {
        Erros(524, Pl);
      }
    }
    return;
  }
  if (Pla == 768){ //ch
    printf("\'%s\'\n", f->d.str);
    return;
  }
  if (Pla == 1280){ //ni
    printf("%i", f->d.i);
    return;
  }
  if (Pla == 1536){ //nd
    printf("%f", f->d.f);
    return;
  }
  if (Pla == 1792 || Pla == 2048) { //oa|ol
    printf("%s", f->d.str);
    return;
  }
  if (Pla == 2560) { //=
    printf("=");
    return;
  }
  if (Pla == 2816) { //;
    printf(";");
    return;
  }
}

void imprimemesmop(Pilha* f){
  Pla = (f->info & 3840);
  if (Pla == 256){ //id
    if (VAR[f->d.i] & 1){ //Id
      printf("i%i", f->d.i);
    } else if (VAR[f->d.i] & 2){
      printf("f%i", f->d.i);
    } else if (VAR[f->d.i] & 4){
      printf("c%i", f->d.i);
    } else {
      printf("Variavel usada na expressao nao foi previamente declarada\n");
    }
    return;
  }
  if (Pla == 3584){
    printf("t%i", f->d.i);
  }
  if (Pla == 768){ //ch
    printf("\'%s\'\n", f->d.str);
    return;
  }
  if (Pla == 1280){ //ni
    printf("%i", f->d.i);
    return;
  }
  if (Pla == 1536){ //nd
    printf("%f", f->d.f);
    return;
  }
  if (Pla == 1792 || Pla == 2048) { //oa|ol
    printf("%s", f->d.str);
    return;
  }
  if (Pla == 2560) { //=
    printf("=");
    return;
  }
  if (Pla == 2816) { //;
    printf(";");
    return;
  }
}

void filatods(TFila* g, TFila* f){
  TFila* h;
  for (h = g; h->info != 3328; h = h->prox) {
    if (h == f){
      printf("<");
    } else {
      printf(" ");
    }
    imprimemesmo(h);
    if (h == f){
      printf(">");
    } else {
      printf(" ");
    }
  }
  printf("\n\n");
}

void vartods(){
  int i;
  printf("VAR = ");
  for (i = 0; i < QVAR; i++) {
    printf("[");
    if (VAR[i] & 1){
      printf("int, ");
    } else if (VAR[i] & 2){
      printf("flt, ");
    } else if (VAR[i] & 4){
      printf("chr, ");
    }
    printf("%i] ", i);
  }
  printf("\n\n");
}

void pilha_imprime2(){
  Pilha* h;
  for (h = pilha; h != NULL; h = h->prox) {
    imprimemesmop(h);
    printf("\n");
  }
  printf("\n");
}

TFila* atri(TFila* f){
  int pilen = 0, i;
  TFila* g;
  g = f;
  int QVARold = QVAR;
  for (i = 0; i < QVAR; i++) {
    if (VAR[i] == 0){
      printf("Alguem nao foi declarado\n");
      exit(-1);
    }
  }
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
    } else {
      Erros(268, Pl);
    }
  } else {
    printf("Nao recebi id da chamada\n");
    exit(-1);
  }
  Pla = (f->info & 3840);
  for (; Pla != 2816; f = f->prox) {
    vartods();
    filatods(g, f);
    Pla = (f->info & 3840);
    if (Pla == 256 || Pla == 1280){
      pilha = pilha_insereint(pilha, f->info, f->d.i);
    } else if (Pla == 1536){
      pilha = pilha_insereflt(pilha, f->info, f->d.f);
    } else if (Pla == 768){
      pilha = pilha_inserestr(pilha, f->info, f->d.str);
    } else if (Pla == 1792 || Pla == 2048){
      imprimemesmop(pilha);
      printf(" %s ", f->d.str);
      pilha = pilha_remove(pilha);
      imprimemesmop(pilha);
      pilha = pilha_remove(pilha);
      pilha = pilha_insereint(pilha, 3584, QVARold);
      printf(" -> ");
      imprimemesmop(pilha);
      printf("\n\n");
      QVARold++;
    } else if (Pla == 2816){
      imprimemesmo(g);
      printf(" = ");
      imprimemesmop(pilha);
      printf("\n\n");
    }
    pilha_imprime2();
    printf("-----------------------\n");
    Pla = (f->info & 3840);
  }
}
