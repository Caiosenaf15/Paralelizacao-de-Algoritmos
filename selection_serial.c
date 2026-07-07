#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <tamanho> <arquivo>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    float *v = (float *)malloc(n * sizeof(float));

    FILE *f = fopen(argv[2], "rb");
    if (!f) { perror("fopen"); return 1; }
    fread(v, sizeof(float), n, f);
    fclose(f);

    double t_inicio = omp_get_wtime();

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[min_idx])
                min_idx = j;
        }
        if (min_idx != i) {
            float tmp = v[i];
            v[i] = v[min_idx];
            v[min_idx] = tmp;
        }
    }

    double t_fim = omp_get_wtime();
    printf("Tempo serial: %.6f s\n", t_fim - t_inicio);

    FILE *out = fopen("vetor_ordenado.out", "wb");
    fwrite(v, sizeof(float), n, out);
    fclose(out);
    free(v);
    return 0;
}
