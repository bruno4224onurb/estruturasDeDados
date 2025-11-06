#include <stdio.h>
#define MAX 100
int in_bounds(int x, int y, int n){
    if ((x<n && x>=0) && (y<n && y>=0)){
        return 1;
    }
    else{
        return 0;
    }
}
void updated_matrix(int M, int origin[][MAX], int destiny[][MAX]){
// iterate and copy from origin to destiny
    int i,j;
    for (i=0;i<M;i++)
        for(j=0;j<M;j++)
            destiny[i][j] = origin[i][j];
}
int count_alive_neighbors(int M,int cells[MAX][MAX], int i, int j){
    int count = 0;
    int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dj[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int d = 0; d < 8; d++) {
        // count alive cells arround cells[i][j]
        if ((in_bounds(i+di[d],j+dj[d],M) == 1) && (cells[i+di[d]][j+dj[d]] == 1)){
            count++;
        }

    }
    return count;
}
// Compute updated temperatures
void compute_updated_temperatures(int M, int temp[][MAX], int temp_next[][MAX]){
    for (int i = 1; i < M-1; i++) {
        for (int j = 1; j < M-1; j++) {
            temp_next[i][j] = (temp[i-1][j]+temp[i+1][j]+temp[i][j-1]+temp[i][j+1])/4;
            // store updated values in temp_next
        }
    }
    for (int i=0; i<M; i++){
        temp_next[i][0] = temp[i][0];
        temp_next[i][M-1] = temp[i][M-1];
        temp_next[0][i] = temp[0][i];
        temp_next[M-1][i] = temp[M-1][i];
    }
    updated_matrix(M,temp_next,temp);
}
// Compute updated cells
void compute_updated_cells(int M,int Tmin, int Tmax, int cells[MAX][MAX], int MatT[MAX][MAX], int cells_next[MAX][MAX]){
    int alive =0;
    for(int i=0;i<M;i++){
        for(int j=0;j<M;j++){
            alive = count_alive_neighbors(M,cells,i,j);
            if ((MatT[i][j]<=Tmax) && (MatT[i][j]>=Tmin)){
                if (cells[i][j] == 1){
                    if (alive>3 || alive<2) // solidao ou superpopulacao
                        cells_next[i][j] = 0;
                    else
                        cells_next[i][j] = 1; // sobrevivencia
                }
                else
                    if (alive==3) // reproducao
                        cells_next[i][j] = 1;
            }
            else{
                cells_next[i][j] = 0;
            } 
        }
    }
    updated_matrix(M,cells_next,cells);
    // iterate and store updated values in cells_next
}

int main(){
    int M,K,Tmin,Tmax,i,j;
    scanf("%d",&M);
    scanf("%d",&K);
    scanf("%d",&Tmin);
    scanf("%d",&Tmax);
    int MatT[MAX][MAX] = {{0}};
    int MatC[MAX][MAX] = {{0}};
    int MatT_next[MAX][MAX] = {{0}};
    int MatC_next[MAX][MAX] = {{0}}; 

    for (i=0;i<M;i++){
        for(j=0;j<M;j++){
            scanf("%d",&MatT[i][j]);
        }
    }
    for (i=0;i<M;i++){
        for(j=0;j<M;j++){
            scanf("%d",&MatC[i][j]);
        }
    }
    for (i=0;i<K;i++){
        compute_updated_temperatures(M,MatT,MatT_next);
        compute_updated_cells(M,Tmin,Tmax,MatC,MatT,MatC_next);
    }
    for (i=0;i<M;i++){
        for(j=0;j<M;j++){
            printf("%d ", MatT[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i=0;i<M;i++){
        for(j=0;j<M;j++){
            printf("%d ", MatC[i][j]);
        }
        printf("\n");
    }
}