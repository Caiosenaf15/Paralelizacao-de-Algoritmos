CC     = gcc
CFLAGS = -O2 -fopenmp -Wall

BINS = matrixmult_serial matrixmult_paralelo \
       selection_serial   selection_paralelo  \
       heapsort_serial    heapsort_paralelo   \
       histograma_serial  histograma_paralelo

all: $(BINS)

matrixmult_serial: matrixmult_serial.c
	$(CC) $(CFLAGS) -o $@ $<

matrixmult_paralelo: matrixmult_paralelo.c
	$(CC) $(CFLAGS) -o $@ $<

selection_serial: selection_serial.c
	$(CC) $(CFLAGS) -o $@ $<

selection_paralelo: selection_paralelo.c
	$(CC) $(CFLAGS) -o $@ $<

heapsort_serial: heapsort_serial.c
	$(CC) $(CFLAGS) -o $@ $<

heapsort_paralelo: heapsort_paralelo.c
	$(CC) $(CFLAGS) -o $@ $<

histograma_serial: histograma_serial.c
	$(CC) $(CFLAGS) -o $@ $<

histograma_paralelo: histograma_paralelo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) *.out *.in

.PHONY: all clean
