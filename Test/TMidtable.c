#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "idtable.h"

Elemento* lista; //Lista
char L[255];

void lista_imprime (Elemento* lista){ //Varre e imprime
  Elemento* p;
  if (lista == NULL){
    printf("Lista Vazia\n");
  }
  for (p = lista; p != NULL; p = p->prox){
    printf("Valor = %i, %s\n", p->info, p->A);
  }
}

void checaId(){
  int E;
  E = lista_procura(lista, L);
  printf ("L = %s - E = %d\n", L, E);
  if (E == -1){
    printf ("if E == -1\n");
    lista = lista_insere(lista, L);
    E = lista_procura(lista, L);
    printf ("E = %d\n", E);
  } else {
    printf("else\n");
  }
  lista_imprime(lista);
}

int main(){
  lista = lista_cria();
  strcpy(L, "Um");
  checaId();
  strcpy(L, "Dois");
  checaId();
  strcpy(L, "Tres");
  checaId();
  strcpy(L, "Um");
  checaId();
  strcpy(L, "Quatro");
  checaId();
  strcpy(L, "Um");
  checaId();
  strcpy(L, "Cinco");
  checaId();
  strcpy(L, "Seis");
  checaId();
  strcpy(L, "Sete");
  checaId();
  strcpy(L, "Oito");
  checaId();
  strcpy(L, "Quatro");
  checaId();
  strcpy(L, "Nove");
  checaId();
  return 0;
}
