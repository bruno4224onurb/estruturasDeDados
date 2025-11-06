#include <stdio.h>
#include <stdlib.h>

typedef struct elemento {
    int valor;
    int linha;
    int coluna;
    struct elemento* prox_coluna;
    struct elemento* prox_linha;
} Elemento;

typedef struct matriz
{
    int tamanho;
    Elemento* cabecalho_linhas;
    Elemento* cabecalho_colunas;
} Matriz;

Matriz* criarMatriz(int n){
    Matriz* m = malloc(sizeof(Matriz));
    m-> tamanho = n;

    Elemento* cabecalho_linhas = NULL;
    Elemento* ultimo_linhas = NULL;

    for(int i=0; i<n;i++){
        Elemento* novo = malloc(sizeof(Elemento));
        novo->valor = 0; //cabeçalho n guarda valor || mesma coisa que escrever (*novo).valor=0
        novo->linha = i;
        novo->coluna = -1;//indicador de cabeçalho
        novo->prox_coluna = NULL;
        novo->prox_linha = NULL;
        if (!cabecalho_linhas)
            cabecalho_linhas = novo;     // o primeiro cabeçalho
        else
            ultimo_linhas->prox_linha = novo; // liga o anterior ao novo

        ultimo_linhas = novo;          // atualiza o último
        }
    m->cabecalho_linhas = cabecalho_linhas; // salva no struct principal
    
    Elemento* cabecalho_colunas = NULL;
    Elemento* ultimo_colunas = NULL;

    for(int j=0; j<n;j++){
        Elemento* novo = malloc(sizeof(Elemento));
        novo->valor = 0; 
        novo->coluna = j;
        novo->linha = -1;
        novo->prox_coluna = NULL;
        novo->prox_linha = NULL;
        if (!cabecalho_colunas)
            cabecalho_colunas = novo;     
        else
            ultimo_colunas->prox_coluna = novo; 

        ultimo_colunas = novo;          
        }
    m->cabecalho_colunas = cabecalho_colunas; 
    
    return m;
}

void inserirMatriz(Matriz* m, int i, int j, int valor){
    if (valor == 0) return; // não insere zeros

    Elemento* novo = malloc(sizeof(Elemento));
    novo->linha = i;
    novo->coluna = j;
    novo->valor = valor;
    novo->prox_coluna = NULL;
    novo->prox_linha = NULL;

    // --- Inserir na linha ---
    Elemento* linha = m->cabecalho_linhas;
    for (int k = 0; k < i; k++) linha = linha->prox_linha;

    Elemento* atual = linha;
    while (atual->prox_coluna && atual->prox_coluna->coluna < j)
        atual = atual->prox_coluna;

    // substitui valor se já existir um nó com mesma posição
    if (atual->prox_coluna && atual->prox_coluna->coluna == j) {
        atual->prox_coluna->valor = valor;
        free(novo);
        return;
    }

    novo->prox_coluna = atual->prox_coluna;
    atual->prox_coluna = novo;

    // --- Inserir na coluna ---
    Elemento* coluna = m->cabecalho_colunas;
    for (int k = 0; k < j; k++) coluna = coluna->prox_coluna;

    Elemento* atual2 = coluna;
    while (atual2->prox_linha && atual2->prox_linha->linha < i)
        atual2 = atual2->prox_linha;

    novo->prox_linha = atual2->prox_linha;
    atual2->prox_linha = novo;
}

void Densa(Matriz* m){
    for(int i = 0; i < m->tamanho; i++){
        Elemento* linha = m->cabecalho_linhas;
        for(int j = 0; j < i; j++){
            linha = linha->prox_linha;
        }
        Elemento* atual = linha->prox_coluna;
        for(int k = 0; k < m->tamanho; k++){
            if(atual && atual->coluna == k){
                printf("%d", atual->valor);
                atual = atual->prox_coluna;  // <-- ESSA LINHA FALTAVA
            } else {
                printf("*");
            }
            if(k < m->tamanho - 1)
                printf(" ");
        }
        printf("\n");
    }
}

int buscarValor(Matriz* m, int i, int j) {
    Elemento* linha = m->cabecalho_linhas;
    for (int k = 0; k < i; k++) linha = linha->prox_linha;

    Elemento* atual = linha->prox_coluna;
    while (atual && atual->coluna < j)
        atual = atual->prox_coluna;

    if (atual && atual->coluna == j)
        return atual->valor;
    return 0;
}

Matriz* multiplicar(Matriz* A, Matriz* B) {
    int n = A->tamanho;
    Matriz* C = criarMatriz(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;
            Elemento* linhaA = A->cabecalho_linhas;
            for (int k = 0; k < i; k++) linhaA = linhaA->prox_linha;
            Elemento* atualA = linhaA->prox_coluna;
            if (!atualA) continue; 
            while (atualA) {
                int valorB = buscarValor(B, atualA->coluna, j);
                if (valorB != 0)
                    soma += atualA->valor * valorB;
                atualA = atualA->prox_coluna;
            }
            if (soma != 0)
                inserirMatriz(C, i, j, soma);
        }
    }
    return C;
}

void liberarMatriz(Matriz* m) {
    Elemento* linha = m->cabecalho_linhas;
    while (linha) {
        Elemento* atual = linha->prox_coluna;
        while (atual) {
            Elemento* prox = atual->prox_coluna;
            free(atual);
            atual = prox;
        }
        Elemento* prox_linha = linha->prox_linha;
        free(linha);
        linha = prox_linha;
    }

    Elemento* coluna = m->cabecalho_colunas;
    while (coluna) {
        Elemento* prox = coluna->prox_coluna;
        free(coluna);
        coluna = prox;
    }

    free(m);
}


int main(){
    int n, kA, kB;
    scanf("%d", &n);

    Matriz* A = criarMatriz(n);
    Matriz* B = criarMatriz(n);

    // leitura da matriz A
    scanf("%d", &kA);
    for (int i = 0; i < kA; i++) {
        int linha, coluna, valor;
        scanf("%d %d %d", &linha, &coluna, &valor);
        inserirMatriz(A, linha - 1, coluna - 1, valor);
    }

    // leitura da matriz B
    scanf("%d", &kB);
    for (int i = 0; i < kB; i++) {
        int linha, coluna, valor;
        scanf("%d %d %d", &linha, &coluna, &valor);
        inserirMatriz(B, linha - 1, coluna - 1, valor);
    }

    // multiplica e imprime
    Matriz* C = multiplicar(A, B);
    Densa(C);

    // libera memória
    liberarMatriz(A);
    liberarMatriz(B);
    liberarMatriz(C);

    return 0;
}
