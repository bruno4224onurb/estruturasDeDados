#include <stdio.h>
#include <stdlib.h>
#define MAX = 1000

typedef struct no {
int chave;
struct no *esq, *dir;
} No;
/*Escreva uma fun¸c˜ao que dada a representa¸c˜ao ligada de uma ´arvore completa, retorna a sua
representa¸c˜ao em um vetor. Suponha que o vetor passado como parˆametro tem espa¸co suficiente
para armazenar a ´arvore.*/
void arv2vet(No *h, int vet[ ], int *n){
    if (h == NULL) return;

    vet[*n] = h->chave;
    int iEsq = (*n) * 2 + 1;
    int iDir = (*n) * 2 + 2;

    arv2vet(h->esq, vet, &iEsq);
    arv2vet(h->dir, vet, &iDir);

}


/*Escreva uma fun¸c˜ao que dada a representa¸c˜ao de uma ´arvore completa em um vetor, retorna a
sua representa¸c˜ao ligada.*/
No *vet2arv(int vet[ ], int n){
    if (vet[n] == 0) {
        return NULL; 
    }

    No * h = (No*)malloc(sizeof(No));
    h->chave = vet[n];

    int esq = n*2 + 1;
    int dir = n*2 + 2;
    h->esq = vet2arv(vet, esq);
    h->dir = vet2arv(vet, dir);
    return h;
}


/*Escreva uma fun¸c˜ao que verifica se uma ´arvore bin´aria com representa¸c˜ao ligada ´e completa.*/ 
int conta_nos(No *arv){
    if (arv == NULL) return 0;
    return 1 + conta_nos(arv->esq) + conta_nos(arv->dir);
}

int checa_complitude(No * arv, int i, int tN){
    if (arv == NULL) return 1;
    if (i>= tN) return 0;
    return checa_complitude(arv->esq, 2*i+1, tN ) && checa_complitude(arv->dir, 2*i+2,tN);
}

int completa(No *arv){
    int total = conta_nos(arv);
    return checa_complitude(arv, 0, total);
}


/*Escreva uma fun¸c˜ao que retorna o valor do elemento m´ınimo em um heap de m´aximo representado em um vetor. Quantas compara¸c˜oes s˜ao necess´arias? Como vocˆe faria para remover este
elemento e manter o heap?*/

int valor_minimo(int heap_max[], int n){
    if (n <= 0) return -1;
    int meio = n/2; /*inicio das folhas*/
    int min = heap_max[meio];

    for (int i = meio; i<n; i++){
        if (heap_max[i] <min){
            min = heap_max[i];
        }
    }

    return min;
}

/*São necessárias no máximo m/2 comparações. Para remover o min e manter o heap eu faria a função abaixo que remove o último e coloca no lugar do mínimo
depois comparo o novo valor de mínimo com seu pai e se for maior subo ele para pai e torno o pai filho*/

void remove_minimo(int heap_max[], int n,  int idx_min){
    heap_max[idx_min] = heap_max[n-1];
    int aux;
    while( (idx_min>0) && (heap_max[idx_min] > heap_max[(idx_min-1)/2])){
        aux = heap_max[(idx_min-1)/2];
        heap_max[(idx_min-1)/2] = heap_max[idx_min];
        heap_max[idx_min] = aux; 
        idx_min = (idx_min-1)/2;
    }
}