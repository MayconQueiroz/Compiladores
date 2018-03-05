#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Erros.h"
#include "Checker.h"
#include "Fila.h"
#include "idtable.h"
#include "PilhasL.h"

/**
* Compilador da linguagem NOME, recebe por entrada codigo.cnm
* e provoca por saida auxiliar.snm - Strings do arquivo .cnm
                      CodInter.inm - Codigo de tres enderecos intermediario
*/

FILE *IN1; //Arquivo de entrada
FILE *OUT0; //Arquivo de strings
FILE *INM; //Arquivo de codigo intermediario
FILE *OASM; //Arquivo asm para codigo assembly

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
int *VVAR; //Tamanho dos vetores VAR
int deb = 0; //Debug mode
int RSTMTC = 0; //Label de saida dos lacos de repeticao (para validacao do brk)
char fltuse = 0; //Uso de floats em atribuicoes
char Eflt = 0; //Atribuicao a floats
char urel = 0; //Uso de relacionais e coisas que o float nao pode
int LGEN = 0; //Quantidade de labels usadas
Pilha* pilha; //Pilha de atribuicoes
int QVARold; //Guarda a antiga situacao do QVAR
char arqab = 0; //Se teve algum fop que abriu um arquivo
int QIFAB = 0; //Quantidade de if's e for's abertos (para evitar abrir um arquivo em um)
int varpos = 0; //Posicao do var para atribuicao do tamanho do vetor
int acevet = 0; //Para armazenamento de qual tipo acessa o vetor

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
    //Verifica chamada de opcoes
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
  if (Fini == Ffin){ //Alguma coisa aconteceu com a fila, mas nao tem nenhuma producao com um token so
    Erros(272, 0);
  }

  Ffin = fila_insereint(Ffin, 3328, 0);

  //////////////////////////////////////////////// Fim Analise Lexica
  //Variaveis que podem ser aproveitadas:
  //int R, Pla, h, I
  //char U, L[256]
  fclose(OUT0);
  OUT0 = fopen("auxiliar.snm", "r");
  if (OUT0 == NULL){
    return Erros(5, 0);
  }
  OASM = fopen("CodAsm.asm", "w");
  if (OASM == NULL){
    return Erros(6, 0);
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
  if (VAR == NULL){ //Falha de alocacao
    return Erros(21, 0);
  }
  VVAR = (int *) calloc(QVAR, sizeof(int));
  if (VVAR == NULL){ //Falha de alocacao
    return Erros(21, 0);
  }
  fprintf(OASM, "section\t.text\n\tglobal _start\n\n_start:\n");

  Ffin = stmt(Fini); //Regras da gramatica

  if (deb == 1){
    for (i = 0; i < QVARold; i++) {
      printf("VAR[%i] = %u. ", i, VAR[i]);
    }
    printf("\n");
  }
  if (!((Ffin->info & 3840) == 3328)){ //Fim da fila em momento inesperado
    Erros(256, Pl);
  }
  //////////////////////////////////////////////// Fim Analise Sintatica e Semantica
  apagaTudo();
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
  if (strcmp(L, "chr") == 0 || strcmp(L, "int") == 0 || strcmp(L, "flt") == 0 || strcmp(L, "brk") == 0 || strcmp(L, "scn") == 0 || strcmp(L, "iff") == 0 || strcmp(L, "els") == 0 || strcmp(L, "for") == 0 || strcmp(L, "whl") == 0 || strcmp(L, "prt") == 0 || strcmp(L, "fpr") == 0 || strcmp(L, "fsc") == 0 || strcmp(L, "fop") == 0){
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

//Declaracao das Funcoes
TFila* mstmt(TFila* f);
TFila* decl_stmt(TFila* f);
TFila* d(TFila* f);
TFila* vetor(TFila* f);
TFila* indexa(TFila* f);
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
//Funcoes para impressao do codigo asm
void asm_mstmt(TFila* f);
void asm_decl_stmt(TFila* f);
void asm_d(TFila* f);
void asm_lexp(TFila* f);
void asm_cond_stmt(TFila* f);
void asm_c(TFila* f, int LGENIF);
void asm_rept_stmt(TFila* f);
void asm_io_stmt(TFila* f);
void asm_iol(TFila* f);
void asm_atri(TFila* f); //Faz todas as impressoes necessarias nao so o asm
void asm_term(TFila* f);
void asm_O(TFila* f);
void asm_R(TFila* f);
void asm_o(TFila* f);
void asm_n(TFila* f);
//Funcoes sem impressao
TFila* _(TFila* f);
TFila* _vetor(TFila* f);
TFila* _indexa(TFila* f);
TFila* _atri(TFila* f);
TFila* _term(TFila* f);
TFila* _O(TFila* f);
TFila* _R(TFila* f);
TFila* _o(TFila* f);
TFila* _n(TFila* f);
//Avanca uma posicao na fila
TFila* checafprox(TFila* f){
  if (f->prox == NULL){
    Erros(256, Pl);
  }
  f = f->prox;
  Pl = Pl + (f->info & 255);
  Pla = (f->info & 3840);
  /*imprimeextenso(Pla);
  if (Pla == 256 || Pla == 1024 || Pla == 1280 || Pla == 3328){
    printf("%i\n", f->d.i);
  } else if (Pla == 1536){
    printf("%f\n", f->d.f);
  } else{
    printf("%s\n", f->d.str);
  }*/
  return f;
}

//Retorna a posicao do vetor acessada pelo Codigo
int posicao(TFila* f){
  int A;
  Pla = (f->info & 3840);
  if (Pla == 3072 && (strcmp(f->d.str, "[") == 0)){ //Delimitador
    f = checafprox(f);
    if (Pla == 256){
      if (VAR[f->d.i] & 5){ //id
        acevet = 12544;
        A = f->d.i;
      } else {
        Erros(277, Pl);
      }
    } else if (Pla == 1280){ //ni
      A = f->d.i;
      acevet = 8448;
    } else if (Pla == 768){ //ch
      A = f->d.str[0];
      acevet = 4352;
    } else {
      Erros(277, Pl);
    }
    if (Pla == 3072 && (strcmp(f->d.str, "]") == 0)){
      return A;
    } else {
      Erros(276, Pl);
    }
  } else {
    acevet = 256;
    return -1;
  }
}

//Verifica e reconhece variaveis (de acordo com a gramatica, e claro)
TFila* checaVar(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 256){
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (VAR[f->d.i] & 1){ //Id
      fprintf(INM, "i%i", f->d.i);
    } else if (VAR[f->d.i] & 2){
      fprintf(INM, "f%i", f->d.i);
    } else if (VAR[f->d.i] & 4){
      fprintf(INM, "c%i", f->d.i);
    }
    f = checafprox(f);
    f = vetor(f);
    return f;
  } else {
    printf("Chamada incorreta de checaVar - Possivel linha %i\n", Pl);
    exit(1);
  }
}

//Checa variaveis para indexacao de vetores
TFila* checaVarnorm(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 256){
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (VAR[f->d.i] & 1){ //Id
      fprintf(INM, "i%i", f->d.i);
    } else if (VAR[f->d.i] & 2){
      fprintf(INM, "f%i", f->d.i);
    } else if (VAR[f->d.i] & 4){
      fprintf(INM, "c%i", f->d.i);
    }
    f = checafprox(f);
    return f;
  } else {
    printf("Chamada incorreta de checaVarnorm - Possivel linha %i\n", Pl);
    exit(1);
  }
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
    } else if (strcmp(f->d.str, "prt") == 0 || strcmp(f->d.str, "scn") == 0 || strcmp(f->d.str, "fsc") == 0 || strcmp(f->d.str, "fpr") == 0 || strcmp(f->d.str, "fop") == 0){
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
  printf("*********%i*****\n", Pla);
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
    } else if (strcmp(f->d.str, "prt") == 0 || strcmp(f->d.str, "scn") == 0 || strcmp(f->d.str, "fsc") == 0 || strcmp(f->d.str, "fpr") == 0 || strcmp(f->d.str, "fop") == 0){
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
    fprintf(INM, "i");
    f = checafprox(f);
    A = 1;
  } else if (strcmp(f->d.str, "flt") == 0){
    fprintf(INM, "f");
    f = checafprox(f);
    A = 2;
  } else if (strcmp(f->d.str, "chr") == 0){
    fprintf(INM, "c");
    f = checafprox(f);
    A = 4;
  } else {
    printf ("Declaracao de variavel esperada\n");
    exit(-1);
  }
  if (Pla == 256){ //ID
    if (VAR[f->d.i] != 0){ //Variavel ja declarada
      Erros(525, Pl);
    }
    VAR[f->d.i] = A;
    varpos = f->d.i;
    VVAR[f->d.i] = 1;
    fprintf(INM, "%i", f->d.i);
  } else {
    Erros(259, Pl);
  }
  f = checafprox(f);
  f = d(f);
  if (Pla == 2816){ //;
    f = checafprox(f);
  } else {
    printf("Erro 6\n");
    Erros(260, Pl);
  }
  Decl = f;
  return f;
}

TFila* d(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 3072 && strcmp(f->d.str, "[") == 0){ //Colchetes
    fprintf(INM, "[");
    f = checafprox(f);
    if (Pla == 1280){ //Inteiro
      if (f->d.i <= 0){
        Erros(523, Pl);
      }
      VVAR[varpos] = f->d.i;
      fprintf(INM, "%i", f->d.i);
      f = checafprox(f);
      if (Pla == 3072 && strcmp(f->d.str, "]") == 0) { //Colchetes
        fprintf(INM, "]\n");
        VAR[varpos] = VAR[varpos] | 16;
        return checafprox(f);
      } else {
        Erros(276, Pl);
      }
    } else {
      Erros(275, Pl);
    }
  } else { //La ele trata o erro
    fprintf(INM, "\n");
    return f;
  }
}

TFila* vetor(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 3072 && (strcmp(f->d.str, "[") == 0)){ //Delimitador
    fprintf(INM, "[");
    f = checafprox(f);
    f = indexa(f);
    if (Pla == 3072 && (strcmp(f->d.str, "]") == 0)){
      fprintf(INM, "]");
      return checafprox(f);
    } else {
      Erros(276, Pl);
    }
  } else {
    return f;
  }
}

TFila* indexa(TFila* f){
  if (Pla == 256){
    if (VAR[f->d.i] & 5){ //id
      f = checaVarnorm(f);
      return f;
    } else {
      Erros(277, Pl);
    }
  } else if (Pla == 1280){ //ni
    fprintf(INM, "%i", f->d.i);
    return checafprox(f);
  } else if (Pla == 768){ //ch
    fprintf(INM, "\'%s\'", f->d.str);
    return checafprox(f);
  } else {
    Erros(277, Pl);
  }
}

TFila* n(TFila* f){
  int i;
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = (f->info & 3840);
  if (Pla == 256){
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (VAR[f->d.i] == 2){ //Uso de float
      fltuse = 1;
    }
    return checaVar(f);
  } else if (Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 1536){
      fltuse = 1;
      fprintf(INM, "%f ", f->d.f);
    } else if (Pla == 1280){
      fprintf(INM, "%i ", f->d.i);
    } else if (Pla == 768){
      fprintf(INM, "\'%c\' ", f->d.str[0]);
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}

TFila* lexp(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Pla = f->info & 3840;
  if (Pla == 256){ //Confirmacao se e um id
    f = checaVar(f);
    fprintf(INM, " ");
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
    QIFAB += 1;
    if (QIFAB < 0){ //Algum valor estourou
      Erros(517, Pl);
    }
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
            QIFAB -= 1;
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
  if (Pla == 512 && strcmp(f->d.str, "els") == 0){ //else
    QIFAB += 1;
    if (QIFAB < 0){ //Algum valor estourou
      Erros(517, Pl);
    }
    f = checafprox(f);
    if (Pla == 3072 && strcmp(f->d.str, "{") == 0){
      f = checafprox(f);
      f = stmt(f);
      if ((f->info & 3840) == 3072 && strcmp(f->d.str, "}") == 0){
        QIFAB -= 1;
        f = checafprox(f);
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
    QIFAB += 1;
    if (QIFAB < 0){ //Algum valor estourou
      Erros(517, Pl);
    }
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
            QIFAB -= 1;
            g = atri(g); //Impressao da atribuicao de passagem
            fprintf(INM, "gto L%i\n:L%i\n", Ind, Ind+2);
            f = checafprox(f);
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
    QIFAB += 1;
    if (QIFAB < 0){ //Algum valor estourou
      Erros(517, Pl);
    }
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
            QIFAB -= 1;
            f = checafprox(f);
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
    printf("=======%i=====\n", Pla);
    if (Pla == 256){ //Id
      VAR[f->d.i] = VAR[f->d.i] | 8;
      f = checafprox(f);
      f = vetor(f);
      printf("Pla = %i..\n", Pla);
      if (Pla == 2816){ //;
        fprintf(INM, "\n");
        f = checafprox(f);
        return f;
      } else {
        printf("Erro 3 - f = %s\n", f->d.str);
        Erros(260, Pl);
      }
    } else {
      Erros(259, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "prt") == 0){
    fprintf(INM, "prt ");
    f = checafprox(f);
    f = iol(f);
    printf("Pla == %i--\n", Pla);
    if (Pla == 2816){ //;
      f = checafprox(f);
      return f;
    } else {
      printf("Erro 4\n");
      Erros(260, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "fsc") == 0){
    if (arqab != 2){ //0 = arquivo nao aberto. 1 = Arquivo aberto em modo escrita
      Erros(518 + arqab, Pl);
    }
    fprintf(INM, "fsc ");
    f = checafprox(f);
    if (Pla == 256){ //Id
      VAR[f->d.i] = VAR[f->d.i] | 8;
      f = (f);
      if (Pla == 2816){ //;
        fprintf(INM, "\n");
        f = checafprox(f);
        return f;
      } else {
        printf("Erro 5\n");
        Erros(260, Pl);
      }
    } else {
      Erros(259, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "fpr") == 0){
    if (arqab != 1){ //0 = arquivo nao aberto. 2 = Arquivo aberto em modo leitura
      Erros(518 + arqab, Pl);
    }
    fprintf(INM, "fpr ");
    f = checafprox(f);
    f = iol(f);
    if (Pla == 2816){ //;
      f = checafprox(f);
      return f;
    } else {
      Erros(260, Pl);
    }
  } else if ((f->info & 3840) == 512 && strcmp(f->d.str, "fop") == 0){
    if (QIFAB != 0){ //Laco de repeticao aberto
      Erros(521, Pl);
    }
    fprintf(INM, "fop ");
    f = checafprox(f);
    if (Pla == 1024){ //String
      fprintf(INM, "\"");
      fscanf(OUT0, "%c", &U);
      while (U != '\n'){
        fprintf(INM, "%c", U);
        fscanf(OUT0, "%c", &U);
      }
      fprintf(INM, "\" ");
      f = checafprox(f);
      if (Pla == 768){ //caractere
        if (strcmp(f->d.str, "r") == 0){
          arqab = 2;
          fprintf(INM, "\'r\'\n");
          f = checafprox(f);
          if (Pla == 2816) { //;
            return checafprox(f);
          } else {
            Erros(260, Pl);
          }
        } else if (strcmp(f->d.str, "w") == 0){
          arqab = 1;
          fprintf(INM, "\'w\'\n");
          f = checafprox(f);
          if (Pla == 2816) { //;
            return checafprox(f);
          } else {
            Erros(260, Pl);
          }
        } else {
          Erros(522, Pl);
        }
      } else {
        Erros(274, Pl);
      }
    } else {
      Erros(273, Pl);
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
    if (Pla == 256){ //Id
      if (VAR[f->d.i] & 16){ //Eh vetor
        f = checafprox(f);
        if (Pla == 3072){ //[
          f = vetor(f);
        } else {
          Erros(0, Pl);
        }
      } else { //Nao eh vetor
        f = checafprox(f);
      }
      return f;
    } else if (Pla == 768) { //Caractere
      fprintf(INM, "\'%s\'\n", f->d.str);
      return checafprox(f);
    } else if (Pla == 1024) { //String
      fprintf(INM, "\"");
      fscanf(OUT0, "%c", &U);
      while (U != '\n'){
        fprintf(INM, "%c", U);
        fscanf(OUT0, "%c", &U);
      }
      fprintf(INM, "\"\n");
      return checafprox(f);
    } else if (Pla == 1280) { //Inteiro
      fprintf(INM, "%i\n", f->d.i);
      return checafprox(f);
    } else if (Pla == 1536) { //Decimal
      fprintf(INM, "%f\n", f->d.f);
      return checafprox(f);
    }
  } else {
    Erros(267, Pl);
  }
}

TFila* atri(TFila* f){
  int pilen = 0, i, VPos;
  TFila* g;
  TFila* G;
  g = f;
  fltuse = 0;
  _atri(f);
  int QVARold = QVAR;
  if ((f->info & 3840) == 256){ //Id
    VAR[f->d.i] = VAR[f->d.i] | 8; //Assegurar que a variavel ja possui valor atribuido
    f = checafprox(f);
    f = vetor(f);
    printf("Pla = %i\n", Pla);
    if (Pla == 2560){ //=
      f = checafprox(f);
    } else {
      printf("Erro 2\n");
      Erros(268, Pl);
    }
  } else {
    printf("Nao recebi id da chamada\n");
    exit(-1);
  }
  for (; Pla != 2816; f = f->prox){ //Ate ;
    Pla = (f->info & 3840);
    if (Pla == 256) { //Id
      G = checafprox(f);
      VPos = posicao(G);
      if (VAR[f->d.i] & 1){ //id int
        ////////////////////(ponteiro, identificacao, valor (id no caso), posicao do vetor
        pilha = pilha_insereint(pilha, acevet, f->d.i, VPos);
      } else if (VAR[f->d.i] & 2){ //id flt
        pilha = pilha_insereint(pilha, acevet, f->d.i, VPos);
      } else if (VAR[f->d.i] & 4){ //id chr
        pilha = pilha_insereint(pilha, acevet, f->d.i, VPos);
      }
    } else if (Pla == 1280){ //int
      pilha = pilha_insereint(pilha, 1280, f->d.i, -1);
    } else if (Pla == 1536){ //flt
      pilha = pilha_insereflt(pilha, 1536, f->d.f, -1);
    } else if (Pla == 768){ //chr
      pilha = pilha_inserestr(pilha, 768, f->d.str, -1);
    } else if ((Pla == 1792 && strcmp(f->d.str, "!") != 0) || Pla == 2048){ //Reducao
      pilha = pilha_remove(pilha);
      pilha = pilha_remove(pilha);
      if (fltuse == 0){ //Nao tem float
        pilha = pilha_insereint(pilha, 3584, QVARold, -1);
      } else { //Tem float
        pilha = pilha_insereint(pilha, 3840, QVARold, -1);
      }
      QVARold++;
    } else if (Pla == 1792 && strcmp(f->d.str, "!") == 0) { //Sem reducao (!)
      pilha = pilha_remove(pilha);
      pilha = pilha_insereint(pilha, 3584, QVARold, -1);
      QVARold++;
    } else if (Pla == 2816){ //;
      return checafprox(f);
    }
  }
}

///////////////////////////////////////Funcoes subtraco (nao imprimem no arquivo de saida)

TFila* _(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 256){
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    f = checafprox(f);
    f = _vetor(f);
    return f;
  } else {
    printf("Chamada incorreta de  - Possivel linha %i\n", Pl);
    exit(1);
  }
}

TFila* _vetor(TFila* f){
  Pla = (f->info & 3840);
  if (Pla == 3072 && (strcmp(f->d.str, "[") == 0)){ //Delimitador
    f = checafprox(f);
    f = _indexa(f);
    if (Pla == 3072 && (strcmp(f->d.str, "]") == 0)){
      return checafprox(f);
    } else {
      Erros(276, Pl);
    }
  } else {
    return f;
  }
}

TFila* _indexa(TFila* f){
  if (Pla == 256){
    if (VAR[f->d.i] & 5){ //id
      f = _(f);
      return f;
    } else {
      Erros(277, Pl);
    }
  } else if (Pla == 1280){ //ni
    return checafprox(f);
  } else if (Pla == 768){ //ch
    return checafprox(f);
  } else {
    Erros(277, Pl);
  }
}

TFila* _atri(TFila* f){
  if (f == NULL){
    Erros(256, Pl);
  }
  Eflt = 0;
  fltuse = 0;
  urel = 0;
  printf("----------%i----\n", Pla);
  if ((f->info & 3840) == 256){ //Id
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if ((VAR[f->d.i] & 2) != 0){ //Marcacao que espera um float
      Eflt = 1;
    }
    VAR[f->d.i] = VAR[f->d.i] | 8;
    f = _(f);
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
        printf("Erro 5\n");
        Erros(260, Pl);
      }
    } else {
      printf("Erro 1\n");
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
  if (Pla == 256){
    if (VAR[f->d.i] == 0){ //id ja foi declarado
      Erros(512, Pl);
    }
    if (VAR[f->d.i] == 2){ //Uso de float
      fltuse = 1;
    }
    return _(f);
  } else if (Pla == 768 || Pla == 1280 || Pla == 1536){
    if (Pla == 1536){
      fltuse = 1;
    }
    return checafprox(f);
  } else {
    Erros(269, Pl);
  }
}

///////////////////////////////////////Funcoes asm (imprimem no arquivo asm codigo assembly)
void asm_decl_stmt(TFila* f){

}

void asm_d(TFila* f){

}

void asm_lexp(TFila* f){

}

void asm_cond_stmt(TFila* f){

}

void asm_rept_stmt(TFila* f){

}

void asm_io_stmt(TFila* f){

}

void asm_iol(TFila* f){

}

void asm_atri(TFila* f){

}

void asm_term(TFila* f){

}

void asm_O(TFila* f){

}

void asm_R(TFila* f){

}

void asm_o(TFila* f){

}

void asm_n(TFila* f){

}
