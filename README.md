# Paralelização de Algoritmos Clássicos com OpenMP

Artigo acadêmico e implementações em C avaliando **Speedup**, **Eficiência** e a **Lei de Amdahl** na paralelização de quatro algoritmos clássicos usando OpenMP.

**Autores:** Lucas Pontes Lira e Caio Sena Freitas
**Instituição:** CEFET/RJ — UNED Nova Friburgo, Bacharelado em Sistemas de Informação

## Sobre o trabalho

O estudo paraleliza quatro algoritmos com características distintas de paralelizabilidade, comparando uma versão serial de referência com uma versão paralela em OpenMP para cada um:

- **Multiplicação de matrizes** — caso quase ideal de paralelização, com cálculos independentes entre si
- **Histograma** — exemplo clássico de condição de corrida, exigindo sincronização cuidadosa
- **Selection Sort** — paralelização limitada por dependências sequenciais
- **Heap Sort** — comparado ao Selection Sort para analisar diferenças de comportamento de cache e balanceamento de carga

Os testes variaram o tamanho das entradas e o número de threads (1, 2 e 4), medindo tempo de execução, speedup e eficiência, e comparando os resultados com os limites teóricos da Lei de Amdahl.

## Estrutura do repositório

```
.
├── artigo_paralelo.tex        # Artigo completo em LaTeX
├── Makefile                   # Compilação de todos os binários
├── matrixmult_serial.c
├── matrixmult_paralelo.c
├── selection_serial.c
├── selection_paralelo.c
├── heapsort_serial.c
├── heapsort_paralelo.c
├── histograma_serial.c
└── histograma_paralelo.c
```

## Tecnologias e ferramentas

- **C** — implementação dos algoritmos
- **OpenMP** — diretivas de paralelização (`#pragma omp parallel for`, `reduction`, `critical`, `atomic`)
- **GCC** com flag `-fopenmp`
- **LaTeX** — redação do artigo

## Compilando e executando

Pré-requisito: `gcc` com suporte a OpenMP.

```bash
# Compilar todos os binários
make

# Executar um binário específico (exemplo)
./matrixmult_serial
./matrixmult_paralelo

# Definir número de threads antes de rodar a versão paralela
export OMP_NUM_THREADS=4
./matrixmult_paralelo

# Limpar os binários gerados
make clean
```

Cada algoritmo possui uma versão `_serial` (referência) e uma versão `_paralelo` (com OpenMP), permitindo comparar diretamente tempo de execução, speedup e eficiência entre as duas.

## Compilando o artigo

O artigo está em `artigo_paralelo.tex` e pode ser compilado com qualquer distribuição LaTeX (TeX Live, MiKTeX) ou via [Overleaf](https://www.overleaf.com/):

```bash
pdflatex artigo_paralelo.tex
```

## Principais métricas avaliadas

- **Tempo de execução (T):** medido com `omp_get_wtime()`
- **Speedup:** S(p) = T_serial / T_paralelo
- **Eficiência:** E(p) = S(p) / p
- **Lei de Amdahl:** limite teórico do speedup dado a fração serial do programa

## Resultados (resumo)

A multiplicação de matrizes e o histograma escalaram bem com o aumento de threads. Os algoritmos de ordenação (Selection Sort e Heap Sort) apresentaram ganhos bem menores, devido a dependências sequenciais inerentes ao algoritmo — resultado consistente com o previsto pela Lei de Amdahl.

## Licença

Trabalho acadêmico desenvolvido para fins educacionais no âmbito do CEFET/RJ.
