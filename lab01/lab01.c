#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int count_alive_neighbors()
{
    int M = 0;
    printf("Tamanho matriz");
    scanf("%d", &M);
    int cells[M][M];
    int i = 0;
    int j = 0;
    int count = 0;
    int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int d = 0; d < 8; d++) {
        // count alive cells arround cells[i][j]
    }
    return count;
}