#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

typedef struct {
    double v;
    double dv;
} Pair;

typedef struct {
    Pair *data;
    size_t top;
    size_t cap;
} Stack;

void stack_init(Stack *s) {
    s->cap = 64;
    s->top = 0;
    s->data = (Pair*)malloc(sizeof(Pair)*s->cap);
    if (!s->data) { perror("malloc"); exit(1); }
}

void stack_free(Stack *s) { free(s->data); s->data = NULL; s->top = s->cap = 0; }

void stack_push(Stack *s, Pair p) {
    if (s->top >= s->cap) {
        s->cap *= 2;
        s->data = (Pair*)realloc(s->data, sizeof(Pair)*s->cap);
        if (!s->data) { perror("realloc"); exit(1); }
    }
    s->data[s->top++] = p;
}

Pair stack_pop(Stack *s) {
    if (s->top == 0) { fprintf(stderr, "Erro: pilha vazia\n"); exit(1); }
    return s->data[--s->top];
}

int is_number_token(const char *tok) {
    char *end;
    if (tok[0] == '\0') return 0;
    errno = 0;
    strtod(tok, &end);
    if (end != tok && *end == '\0' && errno == 0) return 1;
    return 0;
}

int main(void) {
    fseek(stdin, 0, SEEK_END);
    long sz = ftell(stdin);
    fseek(stdin, 0, SEEK_SET);
    if (sz < 0) sz = 1<<20;
    char *buf = (char*)malloc(sz + 1);
    if (!buf) { perror("malloc"); return 1; }
    size_t read = fread(buf,1,sz,stdin);
    buf[read] = '\0';

    char *tok = strtok(buf, " \t\r\n");
    if (!tok) { fprintf(stderr,"Entrada vazia\n"); free(buf); return 1; }
    char **tokens = NULL;
    size_t nt = 0, cap = 0;
    while (tok) {
        if (nt >= cap) {
            cap = cap ? cap*2 : 128;
            tokens = (char**)realloc(tokens, sizeof(char*)*cap);
            if (!tokens) { perror("realloc"); free(buf); return 1; }
        }
        tokens[nt++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }

    if (nt < 2) { fprintf(stderr,"Formato inválido\n"); free(buf); free(tokens); return 1; }

    char *x0tok = tokens[nt-1];
    if (!is_number_token(x0tok)) { fprintf(stderr,"Último token deve ser número\n"); free(buf); free(tokens); return 1; }
    double x0 = strtod(x0tok, NULL);

    Stack st;
    stack_init(&st);

    for (size_t i = 0; i < nt-1; ++i) {
        char *t = tokens[i];
        if (strlen(t) == 1) {
            char c = t[0];
            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                Pair b = stack_pop(&st);
                Pair a = stack_pop(&st);
                Pair res;
                if (c == '+') {
                    res.v = a.v + b.v;
                    res.dv = a.dv + b.dv;
                } else if (c == '-') {
                    res.v = a.v - b.v;
                    res.dv = a.dv - b.dv;
                } else if (c == '*') {
                    res.v = a.v * b.v;
                    res.dv = a.dv * b.v + a.v * b.dv;
                } else if (c == '/') {
                    if (b.v == 0.0) { fprintf(stderr,"Divisão por zero\n"); stack_free(&st); free(buf); free(tokens); return 1; }
                    res.v = a.v / b.v;
                    res.dv = (a.dv * b.v - a.v * b.dv) / (b.v * b.v);
                } else if (c == '^') {
                    if (b.dv != 0.0) {
                        fprintf(stderr,"Expoente depende de x\n");
                        stack_free(&st); free(buf); free(tokens); return 1;
                    }
                    double e = b.v;
                    res.v = pow(a.v, e);
                    res.dv = 0.0;
                    if (a.v > 0.0) {
                        res.dv = e * pow(a.v, e - 1.0) * a.dv;
                    } else if (a.v == 0.0) {
                        if (fabs(e - 1.0) < 1e-15) res.dv = a.dv;
                        else if (e > 1.0) res.dv = 0.0;
                        else res.dv = NAN;
                    } else {
                        double ei;
                        if (modf(e, &ei) == 0.0) {
                            res.dv = e * pow(a.v, e - 1.0) * a.dv;
                        } else {
                            res.v = NAN;
                            res.dv = NAN;
                        }
                    }
                }
                stack_push(&st, res);
                continue;
            }
        }

        if (strcmp(t, "x") == 0) {
            Pair p; p.v = x0; p.dv = 1.0; stack_push(&st, p);
            continue;
        }

        if (is_number_token(t)) {
            Pair p; p.v = strtod(t, NULL); p.dv = 0.0; stack_push(&st, p);
            continue;
        }

        fprintf(stderr,"Token desconhecido: '%s'\n", t);
        stack_free(&st); free(buf); free(tokens); return 1;
    }

    if (st.top != 1) {
        fprintf(stderr,"Erro: pilha final incorreta\n");
        stack_free(&st); free(buf); free(tokens); return 1;
    }

    Pair ans = stack_pop(&st);
    printf("%.5f\n", ans.v);
    printf("%.5f\n", ans.dv);

    stack_free(&st);
    free(buf);
    free(tokens);
    return 0;
}
