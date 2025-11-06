#include <stdio.h>

long long contar_trajetorias(int r, int c, int r_alvo, int c_alvo, int k) {
    if (r < 0 || r > 7 || c < 0 || c > 7) return 0; // fora do tabuleiro
    if (k == 0) return (r == r_alvo && c == c_alvo);

    int dr[8] = {2, 2, -2, -2, 1, 1, -1, -1};
    int dc[8] = {1, -1, 1, -1, 2, -2, 2, -2};

    long long total = 0;
    for (int i = 0; i < 8; i++) {
        total += contar_trajetorias(r + dr[i], c + dc[i], r_alvo, c_alvo, k - 1);
    }
    return total;
}

int main() {
    int r_o, c_o, r_a, c_a, K;

    scanf("%d %d", &r_o, &c_o);
    scanf("%d %d", &r_a, &c_a);
    scanf("%d", &K);

    if (r_o < 0 || r_o > 7 || c_o < 0 || c_o > 7 ||
        r_a < 0 || r_a > 7 || c_a < 0 || c_a > 7 || K < 0) {
        printf("-1\n");
        return 0;
    }

    printf("%lld\n", contar_trajetorias(r_o, c_o, r_a, c_a, K));
    return 0;
}