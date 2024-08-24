CC = gcc
PROGRAM = test
HEIGHT = 16
WIDTH = 16

all: run

.PHONY: run build 

build: src/matrix_lib.c src/timer.c src/matrix_lib_test.c
	$(CC) $(CFLAGS) -o bin/$(PROGRAM) src/matrix_lib.c src/timer.c src/matrix_lib_test.c

run:
	@python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_a.puc -v 2
	@python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_b.puc -v 5
	@echo "Running for size=$(HEIGHT)x$(WIDTH) / algorithm=NotOptimized"
	@./bin/$(PROGRAM) 12.3 $(HEIGHT) $(WIDTH) $(HEIGHT) $(WIDTH) data/input_a.puc data/input_b.puc data/output_1.puc data/output_2.puc

benchmark:
	@rm -f benchmark_output.txt

	@for val in 64 128 256 512 1024 2048; do \
		$(MAKE) run HEIGHT=$$val WIDTH=$$val --no-print-directory | tee -a benchmark_output.txt; \
		clear; \
		python3 plot_benchmark.py < benchmark_output.txt; \
	done

	@rm -f benchmark_output.txt
