#include <stdio.h>
#include <stdlib.h>

typedef struct pilha Pilha;

union Dado {
   int i;
   float f;
   char c;
};

struct pilha{
  int info;
  union Dado d;
  Pilha* prox;
};

struct Pilha{
  int info;
  union Dado d;
  Pilha* prox;
};

Pilha* pilha_insereint(Pilha* pilha, int val, int V){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.i = V;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  return novo;
}

Pilha* pilha_insereflt(Pilha* pilha, int val, float V){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.f = V;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  return novo;
}

Pilha* pilha_inserechr(Pilha* pilha, int val, char V){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.c = V;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  return novo;
}

Pilha* pilha_cria(){ //Retorna Null, criacao de pilhas
  return NULL;
}

/*void pilha_imprime (Pilha* pilha){ //Varre e imprime
  Pilha* p;
  if (pilha == NULL){
    printf("Pilha Vazia\n");
  }
  for (p = pilha; p != NULL; p = p->prox){
    printf("Valor = %i\n",p->info);
  }
}*/

Pilha* pilha_remove (Pilha* pilha){ //Remove a primeira posicao
  Pilha* p;
  Pilha* q;
  if (pilha == NULL){
    printf("Pilha vazia\n");
    return NULL;
  }
  q = pilha;
  pilha = q->prox;
  free(q);
  return pilha;
}

Pilha* pilha_apagar (Pilha* pilha){
  Pilha* p;
  Pilha* q;
  int U = 0;
  if (pilha == NULL){
    return NULL;
  }
  p = pilha;
  while(U == 0){
    q = p->prox;
    free(p);
    p = q;
    if (p == NULL)
      U = 1;
  }
  return NULL;
}

/*int main (){ //Pilhas simplesmente encadeadas
  Pilha* pilha;
  int O, V=0, E, A=0;
  pilha=pilha_cria();
  printf("-----------------------------------\n");
  printf("       PILHAS NAO ESTATICAS\n");
  printf("-----------------------------------\n");
  while(A==0){
    printf("-----------------------------------\n");
    printf("Selecione alguma das opcoes abaixo:\n");
    printf("-----------------------------------\n");
    printf(" 0 - Inserir Elemento\n");
    printf(" 1 - Remover Elemento\n");
    printf(" 2 - Imprimir Pilha\n");
    printf(" 3 - Zerar Pilha\n");
    printf(" 4 - Encerrar Programa\n");
    printf("-----------------------------------\n");
    scanf("%i", &O);
    printf("-----------------------------------\n");
    switch (O) {
      case 0:
        printf("Insira o pilha que deseja inserir na pilha:\n");
        scanf("%i", &V);
        pilha=pilha_insere(pilha, V);
        break;
      case 1:
        pilha=pilha_remove(pilha);
        break;
      case 2:
        pilha_imprime(pilha);
        break;
      case 3:
        pilha = pilha_apagar(pilha);
        break;
      case 4:
        A=1;
        pilha = pilha_apagar(pilha);
        break;
      default:
        printf("Opcao Invalida\n");
    }
  }
  return 0;
}*/
