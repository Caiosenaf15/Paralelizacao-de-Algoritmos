#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

int main(void) {
    FILE *f = fopen("vetor.in", "rb");
    if (!f) { perror("fopen vetor.in"); return 1; }

    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);
    int n = (int)(sz / sizeof(int));

    int *v = (int *)malloc(n * sizeof(int));
    fread(v, sizeof(int), n, f);
    fclose(f);

    /* calcula min e max em paralelo */
    int vmin = INT_MAX, vmax = INT_MIN;
    #pragma omp parallel for reduction(min:vmin) reduction(max:vmax)
    for (int i = 0; i < n; i++) {
        if (v[i] < vmin) vmin = v[i];
        if (v[i] > vmax) vmax = v[i];
    }

    int range = vmax - vmin + 1;
    int *hist  = (int *)calloc(range, sizeof(int));
    int  p     = omp_get_max_threads();

    /* uma copia do histograma por thread para evitar condicao de corrida */
    int **hist_priv = (int **)malloc(p * sizeof(int *));
    for (int t = 0; t < p; t++)
        hist_priv[t] = (int *)calloc(range, sizeof(int));

    double t_inicio = omp_get_wtime();

    #pragma omp parallel
    {
        int  tid     = omp_get_thread_num();
        int *h_local = hist_priv[tid];

        /* cada thread conta sua particao sem conflito */
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++)
            h_local[v[i] - vmin]++;

        /* reducao: soma os histogramas privados no global */
        #pragma omp for schedule(static)
        for (int j = 0; j < range; j++) {
            int soma = 0;
            for (int t = 0; t < p; t++)
                soma += hist_priv[t][j];
            hist[j] = soma;
        }
    }

    double t_fim = omp_get_wtime();
    printf("Tempo paralelo (%d threads): %.6f s\n", p, t_fim - t_inicio);

    FILE *out = fopen("histograma.out", "w");
    for (int j = 0; j < range; j++)
        if (hist[j] > 0)
            fprintf(out, "%d %d\n", j + vmin, hist[j]);
    fclose(out);

    for (int t = 0; t < p; t++) free(hist_priv[t]);
    free(hist_priv); free(v); free(hist);
    return 0;
}
