#include <stdio.h>
long long contador_traj(int r, int c, int r_t, int c_t, int k){
    if(r<0 || r>7 || c<0 || c>7) return 0; //out of bounds
    if(k == 0) return (r == r_t && c==c_t);

    int movr[8] = {2,2,-2,-2,1,1,-1,-1};
    int movc[8] = {1,-1,1,-1,2,-2,2,-2};

    long long total = 0;
    for (int i=0; i <8; i++){
        total += contador_traj(r+movr[i], c +movc[i], r_t, c_t, k-1);
    }
    return total;
}

int main(){
    int setor_r_origem, setor_c_origem, setor_r_alvo, setor_c_alvo, K;
    scanf("%d %d", &setor_r_origem, &setor_c_origem);
    scanf("%d %d", &setor_r_alvo, &setor_c_alvo);
    scanf("%d", &K);

    if(setor_r_origem<0|| setor_c_origem<0|| setor_c_alvo<0|| setor_r_alvo<0||
         setor_r_origem>7|| setor_c_origem>7|| setor_c_alvo>7|| setor_c_alvo>0||
        K<0 || K>12 ){
            printf("-1\n");
            return 0;
        }
    
    printf("%lld\n", contador_traj(setor_r_origem, setor_c_origem, setor_r_alvo, setor_c_alvo, K ));
    return 0;

}