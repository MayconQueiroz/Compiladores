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

void fila_imprime (TFila* fila){ //Varre e imprime
  TFila* p;
  if (fila == NULL){
    printf("Fila Vazia\n");
  }
  for (p = fila; p != NULL; p = p->prox){
    printf("Tipo = %i\t",p->info);
    if (p->info == 0 || p->info == 3 || p->info == 4){
      printf("%i\n", p->d.i);
    } else if (p->info == 5){
      printf("%f\n", p->d.f);
    } else{
      printf("%s\n", p->d.str);
    }
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
