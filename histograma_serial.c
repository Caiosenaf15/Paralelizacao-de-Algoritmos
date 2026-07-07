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

    int vmin = INT_MAX, vmax = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (v[i] < vmin) vmin = v[i];
        if (v[i] > vmax) vmax = v[i];
    }

    int range = vmax - vmin + 1;
    int *hist = (int *)calloc(range, sizeof(int));

    double t_inicio = omp_get_wtime();

    for (int i = 0; i < n; i++)
        hist[v[i] - vmin]++;

    double t_fim = omp_get_wtime();
    printf("Tempo serial: %.6f s\n", t_fim - t_inicio);

    FILE *out = fopen("histograma.out", "w");
    for (int i = 0; i < range; i++)
        if (hist[i] > 0)
            fprintf(out, "%d %d\n", i + vmin, hist[i]);
    fclose(out);

    free(v); free(hist);
    return 0;
}
