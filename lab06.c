#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct node {
    int val;
    struct node *left, *right;
} Node;

void read_input(int *in, int *pos, int n){
    int i;
    for(i=0;i<n;i++) scanf("%d",&in[i]);
    for(i=0;i<n;i++) scanf("%d",&pos[i]);
}

int find_idx(int *v, int L, int R, int x){
    int i;
    for(i=L;i<=R;i++) if(v[i]==x) return i;
    return -1;
}

Node* build(int *in, int *pos, int inL, int inR, int posL, int posR){
    if(inL > inR) return NULL;

    int v = pos[posR];
    Node *r = malloc(sizeof(Node));
    r->val = v;
    r->left = r->right = NULL;

    int idx = find_idx(in, inL, inR, v);
    int t = idx - inL;

    r->left  = build(in, pos, inL, idx-1, posL, posL+t-1);
    r->right = build(in, pos, idx+1, inR, posL+t, posR-1);

    return r;
}

int best_global;

int dfs(Node *r){
    if(!r) return 0;

    int L = dfs(r->left);
    int R = dfs(r->right);

    int best = r->val;
    if(r->val + L > best) best = r->val + L;
    if(r->val + R > best) best = r->val + R;

    int tot = best;
    if(r->val + L + R > tot) tot = r->val + L + R;

    if(tot > best_global) best_global = tot;

    return best;
}

int max_path_sum(Node *r){
    best_global = INT_MIN;
    dfs(r);
    return best_global;
}

void free_tree(Node *r){
    if(!r) return;
    free_tree(r->left);
    free_tree(r->right);
    free(r);
}

int main(){
    int n;
    scanf("%d",&n);

    int *in  = malloc(n*sizeof(int));
    int *pos = malloc(n*sizeof(int));

    read_input(in, pos, n);

    Node *root = build(in, pos, 0, n-1, 0, n-1);

    int ans = max_path_sum(root);
    printf("%d\n", ans);

    free(in);
    free(pos);
    free_tree(root);

    return 0;
}
