#include <stdio.h>
#include <stdlib.h>

typedef struct pilha Pilha;

union Dado2 {
   short i;
   float f;
   char str[4];
};

struct pilha{
  short info;
  int pos;
  union Dado2 d;
  Pilha* prox;
};

struct Pilha{
  int info;
  int pos;
  union Dado2 d;
  Pilha* prox;
};

void pilha_imprime (Pilha* pilha);

//Pilha* pilha - Ponteiro para a pilha_apagar
//int val - Identificacao para tipagem
//int V - Valor (posicao do id ou inteiro/float/char armazenado)
//int P - Vector only (Posicao do vetor a ser acessada (para id's vetores))
Pilha* pilha_insereint(Pilha* pilha, int val, int V, int P){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.i = V;
  novo->pos = P;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  /*printf("Insereint\n");
  pilha_imprime(pilha);*/
  return novo;
}

Pilha* pilha_insereflt(Pilha* pilha, int val, float V, int P){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  novo->d.f = V;
  novo->pos = P;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  /*printf("Insereflt\n");
  pilha_imprime(pilha);*/
  return novo;
}

Pilha* pilha_inserestr(Pilha* pilha, int val, char* V, int P){
  Pilha* novo = (Pilha*) malloc(sizeof(Pilha)); //Reserva um espaco para novo
  novo->info = val; //Atribui a novo, o valor passado
  strcpy(novo->d.str, V);
  novo->pos = P;
  novo->prox = pilha; //Poe o ponteiro para o novo apontando para o pilha anterior da pilha
  /*printf("Inserestr\n");
  pilha_imprime(pilha);*/
  return novo;
}

Pilha* pilha_cria(){ //Retorna Null, criacao de pilhas
  return NULL;
}

void imprimetipop (int V){
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
    printf("Id int Temp ");
    return;
  }
  if (V == 3840){
    printf("Id flt Temp ");
    return;
  }
}

void pilha_imprime (Pilha* pilha){ //Varre e imprime
  Pilha* p;
  int Ax;
  if (pilha == NULL){
    printf("Pilha Vazia\n");
  }
  for (p = pilha; p != NULL; p = p->prox){
    imprimetipop(p->info);
    Ax = p->info & 3840;
    printf("\t%i\t", Ax);
    if (Ax == 256 || Ax == 1024 || Ax == 1280 || Ax == 3584 || Ax == 3840){
      printf("%i\n", p->d.i);
    } else if (Ax == 1536){
      printf("%f\n", p->d.f);
    } else{
      printf("%s\n", p->d.str);
    }
  }
}

Pilha* pilha_remove (Pilha* pilha){ //Remove a primeira posicao
  Pilha* p;
  Pilha* q;
  if (pilha == NULL){
    //printf("Pilha vazia\n");
    return NULL;
  }
  q = pilha;
  pilha = q->prox;
  free(q);
  /*printf("remove\n");
  pilha_imprime(pilha);*/
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
