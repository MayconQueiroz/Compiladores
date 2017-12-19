#include <stdio.h>
#include <stdlib.h>

typedef struct fila TFila;

union Dado {
   int i;
   float f;
   char str[4];
};

struct fila{
  int info;
  union Dado d;
  TFila* prox;
};

struct TFila{
  int info;
  union Dado d;
  TFila* prox;
};

TFila* fila_insereint(TFila* f, int val, int V){ //Insere no ponteiro
  TFila* novo = (TFila*) malloc(sizeof(TFila)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->prox = NULL;
  novo->d.i = V;
  if (f == NULL){
    f = novo;
  } else {
    f->prox = novo;
  }
  return novo;
}

TFila* fila_insereflt(TFila* f, int val, float V){ //Insere no ponteiro
  TFila* novo = (TFila*) malloc(sizeof(TFila)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.f = V;
  novo->prox = NULL;
  if (f == NULL){
    f = novo;
  } else {
    f->prox = novo;
  }
  return novo;
}

TFila* fila_inserestr(TFila* f, int val, char* V){ //Insere no ponteiro
  TFila* novo = (TFila*) malloc(sizeof(TFila)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  strcpy(novo->d.str, V);
  novo->prox = NULL;
  if (f == NULL){
    f = novo;
  } else {
    f->prox = novo;
  }
  return novo;
}

TFila* fila_cria(){ //Retorna Null, criacao de filas
  return NULL;
}

TFila* fila_remove (TFila* fila){ //Remove a primeira posicao
  TFila* q;
  if (fila == NULL){
    printf("Fila vazia\n");
    return NULL;
  }
  q = fila;
  fila = q->prox;
  free(q);
  return fila;
}

void imprimetipo (int V){
  V = V & 3840;
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
    printf("String \t");
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

void fila_imprime (TFila* fila){ //Varre e imprime
  TFila* p;
  int Lin = 1, Ax;
  if (fila == NULL){
    printf("Fila Vazia\n");
    return ;
  }
  printf("Identificador\tLinha\tValor\n");
  for (p = fila; p != NULL; p = p->prox){
    Lin = Lin + (p->info & 255);
    imprimetipo(p->info);
    Ax = p->info & 3840;
    printf("\t%i\t", Lin);
    if (Ax == 256 || Ax == 1024 || Ax == 1280 || Ax == 3328){
      printf("%i\n", p->d.i);
    } else if (Ax == 1536){
      printf("%f\n", p->d.f);
    } else{
      printf("%s\n", p->d.str);
    }
  }
}

void imprimevalor (TFila* fila, int Lin){
  int Ax;
  imprimetipo(fila->info);
  Ax = fila->info & 3840;
  printf("%i\t", Lin);
  if (Ax == 256 || Ax == 1024 || Ax == 1280 || Ax == 3328){
    printf("%i\t", fila->d.i);
  } else if (Ax == 1536){
    printf("%f\t", fila->d.f);
  } else{
    printf("%s\t", fila->d.str);
  }
}

TFila* fila_apagar (TFila* fila){ //Apaga a fila
  TFila* p;
  TFila* q;
  int U = 0;
  if (fila == NULL){
    return NULL;
  }
  p = fila;
  while(U == 0){
    q = p->prox;
    free(p);
    p = q;
    if (p == NULL)
      U = 1;
  }
  return NULL;
}
