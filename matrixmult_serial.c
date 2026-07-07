#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Acessa elemento (i,j) de uma matriz armazenada como vetor 1-D */
#define M(i,j,n,mat) ((mat)[(i)*(n)+(j)])

/* Carrega matriz n x n de doubles de arquivo binario */
double *load_double_matrix(const char *path, int rows, int cols) {
    double *m = (double *)malloc(rows * cols * sizeof(double));
    FILE *f = fopen(path, "rb");
    if (!f) { perror("fopen"); exit(1); }
    fread(m, sizeof(double), rows * cols, f);
    fclose(f);
    return m;
}

/* Salva matriz n x n de doubles em arquivo binario */
void save_double_matrix(const double *m, int rows, int cols, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) { perror("fopen"); exit(1); }
    fwrite(m, sizeof(double), rows * cols, f);
    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <ordem> <arq_A> <arq_B>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    double *A = load_double_matrix(argv[2], n, n);
    double *B = load_double_matrix(argv[3], n, n);
    double *C = (double *)calloc(n * n, sizeof(double));

    double t_inicio = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double soma = 0.0;
            for (int k = 0; k < n; k++)
                soma += M(i, k, n, A) * M(k, j, n, B);
            M(i, j, n, C) = soma;
        }
    }

    double t_fim = omp_get_wtime();
    printf("Tempo serial: %.6f s\n", t_fim - t_inicio);

    save_double_matrix(C, n, n, "matriz_mult.out");
    free(A); free(B); free(C);
    return 0;
}
