#include <stdio.h>
#include <stdlib.h>


typedef struct No{
  int info;
  struct No *prox;
} No;

typedef struct Pilha{
  No *top;
}Pilha;

int radixSort( No *lista){
  Pilha *p0 = (Pilha*)malloc(sizeof(Pilha));
  for(No *aux=lista; aux ->prox != NULL; aux=aux->prox){
    if (aux->info % 10 == 1)
      No * no =(No*)malloc(sizeof(No));
      no->info = aux->info;
      if(p0->top){
        no->prox = p0->top;
        p0->top = no;
      }
      else {
        no->prox = NULL;
        p1->top = no;
      }
  }  
}
