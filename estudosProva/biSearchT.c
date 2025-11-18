#include <stdio.h>
#include <stdlib.h>


typedef struct no {
int chave;
struct no *esq, *dir;
} No;

/*Escreva uma fun¸c˜ao n˜ao-recursiva que recebe uma ´arvore bin´aria de busca t como parˆametro e
retorna o apontador para o n´o cuja chave possui o valor m´ınimo ou NULL caso a ´arvore esteja
vazia.*/
No *minimo(No *t){
    if (t == NULL) return NULL;
    No *min = t;
    for(min; min->esq != NULL; min = min->esq);
    return min;
}

/* Escreva uma fun¸c˜ao n˜ao-recursiva que verifica a existˆencia de algum elemento com chave negativa
na ´arvore.*/
int existe_chave_negativa(No *t){
    if (t== NULL) return 0;
    while (t->esq!= NULL){
        t = t->esq;
    }
    return (t->chave < 0);
}

/*Escreva uma fun¸c˜ao que verifica se uma ´arvore de busca ´e v´alida.*/
/*não considera caso que o nó esteja entre o mínimo e máximo*/
int verifica_busca(No *t){
    if (t == NULL) return 1;
    if((t->esq != NULL) && (t->esq->chave >= t->chave)) return 0;
    if ((t->dir != NULL) &&(t->dir->chave <= t->chave)) return 0;
    return verifica_busca(t->esq) && verifica_busca(t->dir);
}

/*Escreva uma fun¸c˜ao que percorre uma ´arvore de busca e retorna uma lista ligada ordenada
contendo todos os elementos da ´arvore. A sua fun¸c˜ao deve ter complexidade proporcional ao
n´umero de n´os da ´arvore (O(n)). Vocˆe pode usar fun¸c˜oes auxiliares.*/

typedef struct no_lista {
int chave;
struct no_lista* prox;
} No_lista;

No *min(No* t){

}

No *max(No* t){

}

void listaAux(No* t, No_lista **min, No_lista **max){
    if (t == NULL) return;

    listaAux(t->esq,min,max);

    No_lista *aux = (No_lista*)malloc(sizeof(No_lista));
    aux ->chave = t->chave;
    aux->prox = NULL;

    if (*min == NULL){
        *min = aux;
        *max = aux;
    }
    else{
        (*max)->prox = aux;
        *max = aux;
    }

    listaAux(t->dir,min,max);
}

No_lista *lista(No *t){
   No_lista *min = NULL;
   No_lista *max = NULL;
   listaAux(t,&min,&max);
   return min;
}


