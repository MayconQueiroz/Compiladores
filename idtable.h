#include <stdio.h>
#include <stdlib.h>

typedef struct elemento Elemento;

struct elemento{
  int info;
  char A[255];
  Elemento* prox;
};

struct Elemento{
  int info;
  char A[255];
  Elemento* prox;
};

Elemento* lista_insere(Elemento* lista, char* val){ //Antigo metodo de insercao
  Elemento* novo = (Elemento*) malloc(sizeof(Elemento)); //Reserva um espaço para novo
  strcpy(novo->A, val);
  if (lista == NULL){
    novo->info = 0;
  } else {
    novo->info = lista->info + 1;
  }
  novo->prox = lista; //Poe o ponteiro para o novo apontando para o elemento anterior da lista
  return novo;
}

Elemento* lista_cria(){ //Retorna Null, criação de listas
  return NULL;
}

Elemento* lista_apagar (Elemento* lista){
  Elemento* p;
  Elemento* q;
  int U = 0;
  if (lista == NULL){
    return NULL;
  }
  p = lista;
  while(U == 0){
    q = p->prox;
    free(p);
    p = q;
    if (p == NULL)
      U = 1;
  }
  return NULL;
}

int lista_procura (Elemento* lista, char* V){
  Elemento* p;
  for (p=lista; p!=NULL; p=p->prox){
    if (strcmp(p->A, V) == 0) {
      return p->info;
    }
  }
  return -1;
}
