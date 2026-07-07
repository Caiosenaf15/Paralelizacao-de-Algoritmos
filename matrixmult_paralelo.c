#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define M(i,j,n,mat) ((mat)[(i)*(n)+(j)])

double *load_double_matrix(const char *path, int rows, int cols) {
    double *m = (double *)malloc(rows * cols * sizeof(double));
    FILE *f = fopen(path, "rb");
    if (!f) { perror("fopen"); exit(1); }
    fread(m, sizeof(double), rows * cols, f);
    fclose(f);
    return m;
}

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

    /* paralleliza o laco externo: cada thread processa um bloco de linhas */
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double soma = 0.0;
            for (int k = 0; k < n; k++)
                soma += M(i, k, n, A) * M(k, j, n, B);
            M(i, j, n, C) = soma;
        }
    }

    double t_fim = omp_get_wtime();
    printf("Tempo paralelo (%d threads): %.6f s\n",
           omp_get_max_threads(), t_fim - t_inicio);

    save_double_matrix(C, n, n, "matriz_mult.out");
    free(A); free(B); free(C);
    return 0;
}
