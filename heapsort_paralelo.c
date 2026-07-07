#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

static void sift_down(float *v, int i, int sz) {
    while (1) {
        int maior = i;
        int esq   = 2 * i + 1;
        int dir   = 2 * i + 2;
        if (esq < sz && v[esq] > v[maior]) maior = esq;
        if (dir < sz && v[dir] > v[maior]) maior = dir;
        if (maior == i) break;
        float tmp = v[i]; v[i] = v[maior]; v[maior] = tmp;
        i = maior;
    }
}

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

    /*
     * Fase 1 paralela: nos do mesmo nivel da arvore heap sao
     * independentes entre si, entao paralelizamos dentro de cada nivel
     * com uma barreira implicita entre niveis.
     */
    int nivel_atual = n / 2 - 1;
    int nivel_fim;

    while (nivel_atual >= 0) {
        nivel_fim = nivel_atual;
        if (nivel_atual > 0)
            nivel_atual = nivel_atual / 2 - 1
                          + (nivel_atual % 2 == 0 ? -1 : 0);

        #pragma omp parallel for schedule(static)
        for (int i = nivel_fim; i >= nivel_atual + 1; i--)
            sift_down(v, i, n);

        if (nivel_atual < 0) break;
        nivel_atual--;
    }
    sift_down(v, 0, n);  /* processa a raiz */

    /* fase 2: extracao -- serial por dependencia de dados */
    for (int i = n - 1; i > 0; i--) {
        float tmp = v[0]; v[0] = v[i]; v[i] = tmp;
        sift_down(v, 0, i);
    }

    double t_fim = omp_get_wtime();
    printf("Tempo paralelo (%d threads): %.6f s\n",
           omp_get_max_threads(), t_fim - t_inicio);

    FILE *out = fopen("vetor_ordenado.out", "wb");
    fwrite(v, sizeof(float), n, out);
    fclose(out);
    free(v);
    return 0;
}
