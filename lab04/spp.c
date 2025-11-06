#include <stdio.h>
#include <stdlib.h>

// =============================
// Estrutura básica do elemento
// =============================
typedef struct elemento {
    int valor;
    int linha;
    int coluna;
    struct elemento* prox_coluna; // próximo na mesma linha
    struct elemento* prox_linha;  // próximo na mesma coluna
} Elemento;

// =============================
// Estrutura principal da matriz
// =============================
typedef struct {
    int tamanho;
    Elemento* cabeca_linhas;
    Elemento* cabeca_colunas;
} Matriz;

// ===========================================================
// Criação da matriz esparsa com cabeçalhos de linha e coluna
// ===========================================================
Matriz* criarMatriz(int n) {
    Matriz* m = malloc(sizeof(Matriz));
    m->tamanho = n;

    // cria cabeçalhos de linha
    Elemento* cabeca_linhas = NULL;
    Elemento* ultimo_linha = NULL;
    for (int i = 0; i < n; i++) {
        Elemento* novo = malloc(sizeof(Elemento));
        novo->valor = 0;
        novo->linha = i;
        novo->coluna = -1;
        novo->prox_coluna = NULL;
        novo->prox_linha = NULL;

        if (!cabeca_linhas)
            cabeca_linhas = novo;
        else
            ultimo_linha->prox_linha = novo;
        ultimo_linha = novo;
    }
    m->cabeca_linhas = cabeca_linhas;

    // cria cabeçalhos de coluna
    Elemento* cabeca_colunas = NULL;
    Elemento* ultimo_coluna = NULL;
    for (int j = 0; j < n; j++) {
        Elemento* novo = malloc(sizeof(Elemento));
        novo->valor = 0;
        novo->linha = -1;
        novo->coluna = j;
        novo->prox_coluna = NULL;
        novo->prox_linha = NULL;

        if (!cabeca_colunas)
            cabeca_colunas = novo;
        else
            ultimo_coluna->prox_coluna = novo;
        ultimo_coluna = novo;
    }
    m->cabeca_colunas = cabeca_colunas;

    return m;
}

// ====================================================
// Inserção de um elemento (não armazena zeros)
// ====================================================
void inserirElemento(Matriz* m, int i, int j, int valor) {
    if (valor == 0) return;

    Elemento* novo = malloc(sizeof(Elemento));
    novo->valor = valor;
    novo->linha = i;
    novo->coluna = j;
    novo->prox_coluna = NULL;
    novo->prox_linha = NULL;

    // insere na linha
    Elemento* linha = m->cabeca_linhas;
    for (int k = 0; k < i; k++) linha = linha->prox_linha;

    Elemento* atual = linha;
    while (atual->prox_coluna && atual->prox_coluna->coluna < j)
        atual = atual->prox_coluna;

    novo->prox_coluna = atual->prox_coluna;
    atual->prox_coluna = novo;

    // insere na coluna
    Elemento* coluna = m->cabeca_colunas;
    for (int k = 0; k < j; k++) coluna = coluna->prox_coluna;

    atual = coluna;
    while (atual->prox_linha && atual->prox_linha->linha < i)
        atual = atual->prox_linha;

    novo->prox_linha = atual->prox_linha;
    atual->prox_linha = novo;
}

// ====================================================
// Função para buscar valor (retorna 0 se não existir)
// ====================================================
int buscarValor(Matriz* m, int i, int j) {
    Elemento* linha = m->cabeca_linhas;
    for (int k = 0; k < i; k++) linha = linha->prox_linha;

    Elemento* atual = linha->prox_coluna;
    while (atual && atual->coluna < j)
        atual = atual->prox_coluna;

    if (atual && atual->coluna == j)
        return atual->valor;
    return 0;
}

// ====================================================
// Multiplicação de duas matrizes esparsas: C = A × B
// ====================================================
Matriz* multiplicar(Matriz* A, Matriz* B) {
    int n = A->tamanho;
    Matriz* C = criarMatriz(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int soma = 0;
            // percorre elementos não nulos da linha i de A
            Elemento* linhaA = A->cabeca_linhas;
            for (int k = 0; k < i; k++) linhaA = linhaA->prox_linha;

            Elemento* atualA = linhaA->prox_coluna;
            while (atualA) {
                int valorB = buscarValor(B, atualA->coluna, j);
                if (valorB != 0)
                    soma += atualA->valor * valorB;
                atualA = atualA->prox_coluna;
            }
            if (soma != 0)
                inserirElemento(C, i, j, soma);
        }
    }
    return C;
}

// ====================================================
// Impressão densa (usa * para zeros)
// ====================================================
void imprimirMatrizDensa(Matriz* m) {
    for (int i = 0; i < m->tamanho; i++) {
        Elemento* linha = m->cabeca_linhas;
        for (int k = 0; k < i; k++) linha = linha->prox_linha;

        Elemento* atual = linha->prox_coluna;
        for (int j = 0; j < m->tamanho; j++) {
            if (atual && atual->coluna == j) {
                printf("%d", atual->valor);
                atual = atual->prox_coluna;
            } else {
                printf("*");
            }
            if (j < m->tamanho - 1) printf(" ");
        }
        printf("\n");
    }
}

// ====================================================
// Liberação da memória alocada
// ====================================================
void liberarMatriz(Matriz* m) {
    Elemento* linha = m->cabeca_linhas;
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

    Elemento* coluna = m->cabeca_colunas;
    while (coluna) {
        Elemento* prox = coluna->prox_coluna;
        free(coluna);
        coluna = prox;
    }

    free(m);
}

// ====================================================
// Função principal: leitura, multiplicação e impressão
// ====================================================
int main() {
    int n, kA, kB;
    scanf("%d", &n);

    Matriz* A = criarMatriz(n);
    Matriz* B = criarMatriz(n);

    // leitura da matriz A
    scanf("%d", &kA);
    for (int i = 0; i < kA; i++) {
        int linha, coluna, valor;
        scanf("%d %d %d", &linha, &coluna, &valor);
        inserirElemento(A, linha - 1, coluna - 1, valor);
    }

    // leitura da matriz B
    scanf("%d", &kB);
    for (int i = 0; i < kB; i++) {
        int linha, coluna, valor;
        scanf("%d %d %d", &linha, &coluna, &valor);
        inserirElemento(B, linha - 1, coluna - 1, valor);
    }

    // multiplica e imprime
    Matriz* C = multiplicar(A, B);
    imprimirMatrizDensa(C);

    // libera memória
    liberarMatriz(A);
    liberarMatriz(B);
    liberarMatriz(C);

    return 0;
}