#include<stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct No
{
    int val;
    int bal;
    struct No *esq;
    struct No *dir;
}No;


int calc_alt( No * T){
    int h_esq, h_dir;
    if (T == NULL) return 0;
    h_esq = calc_alt(T->esq);
    h_dir = calc_alt(T->dir);
    T->bal = h_esq - h_dir;
    return 1 + (h_dir > h_esq ? h_dir : h_esq);
}

void calc_bal(No* T){
    calc_alt(T);
}

int max_bal(No* T){
    int bal_esq, bal_dir;
    if (T == NULL) return INT_MIN;
    bal_esq = max_bal(T->esq);
    bal_dir = max_bal(T->dir);
    int max = T->bal;
    max = (bal_dir > max) ? bal_dir : max;
    max = (bal_esq > max) ? bal_esq : max;
    return max;
}