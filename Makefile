CC = gcc
PROGRAM = test
HEIGHT = 16
WIDTH = 16
SCALAR = 12.3
INPUT_A = data/input_a.puc
INPUT_B = data/input_b.puc
OUTPUT_1 = data/output_1.puc
OUTPUT_2 = data/output_2.puc


all: run

.PHONY: run build 

build: src/matrix_lib.c src/timer.c src/matrix_lib_test.c
	$(CC) $(CFLAGS) -o bin/$(PROGRAM) src/matrix_lib.c src/timer.c src/matrix_lib_test.c

run:
	@python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_a.puc -v 2
	@python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_b.puc -v 5
	@echo "Running for size=$(HEIGHT)x$(WIDTH) / algorithm=NotOptimized"
	@./bin/$(PROGRAM) $(SCALAR) $(HEIGHT) $(WIDTH) $(HEIGHT) $(WIDTH) $(INPUT_A) $(INPUT_B) $(OUTPUT_1) $(OUTPUT_2)
	@python3 check_results.py $(HEIGHT) $(WIDTH) $(SCALAR) $(INPUT_A) $(INPUT_B) $(OUTPUT_2)

benchmark:
	$(MAKE) build --no-print-directory
	@rm -f benchmark_output.txt

	@for val in 64 128 256 512 1024 2048; do \
		$(MAKE) run HEIGHT=$$val WIDTH=$$val --no-print-directory | tee -a benchmark_output.txt; \
		clear; \
		python3 plot_benchmark.py < benchmark_output.txt; \
	done

	@rm -f benchmark_output.txt
