CC = gcc
PROGRAM = test
HEIGHT = 512
WIDTH = 512
SCALAR = 5.0
LIB_FILES = matrix_lib.c matrix_lib_columns.c
LIB = matrix_lib

define INPUT_A
data/input_a_$(HEIGHT)x$(WIDTH).puc
endef

define INPUT_B
data/input_b_$(HEIGHT)x$(WIDTH).puc
endef

define OUTPUT_1
data/output_1_$(HEIGHT)x$(WIDTH)_$(LIB).puc
endef

define OUTPUT_2
data/output_2_$(HEIGHT)x$(WIDTH)_$(LIB).puc
endef

all: run_all

.PHONY: run_all build_all benchmark clean

build_all: $(LIB_FILES:%.c=bin/$(PROGRAM)_%)

bin/$(PROGRAM)_%: src/%.c src/timer.c src/matrix_lib_test.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

run_all: build_all
	@for lib in $(LIB_FILES:%.c=%); do \
		$(MAKE) run LIB=$$lib --no-print-directory; \
	done

run:
	@if [ ! -f $(INPUT_A) ]; then \
	    python3 input_generator.py $(HEIGHT) $(WIDTH) $(INPUT_A) -v 2; \
	fi	
	@if [ ! -f $(INPUT_B) ]; then \
	    python3 input_generator.py $(HEIGHT) $(WIDTH) $(INPUT_B) -v 5; \
	fi

	@echo "Running for size=$(HEIGHT)x$(WIDTH) / algorithm=$(LIB)"
	@./bin/$(PROGRAM)_$(LIB) $(SCALAR) $(HEIGHT) $(WIDTH) $(HEIGHT) $(WIDTH) $(INPUT_A) $(INPUT_B) $(OUTPUT_1) $(OUTPUT_2)
	@python3 check_results.py $(HEIGHT) $(WIDTH) $(SCALAR) $(INPUT_A) $(INPUT_B) $(OUTPUT_2) $(LIB)

benchmark:
	@$(MAKE) clean
	@$(MAKE) build_all --no-print-directory
	@rm -f benchmark_output.txt
	@for val in 512 1024 2048; do \
		for lib in $(LIB_FILES:%.c=%); do \
			$(MAKE) run HEIGHT=$$val WIDTH=$$val LIB=$$lib --no-print-directory | tee -a benchmark_output.txt; \
		done; \
		clear; \
		python3 plot_benchmark.py < benchmark_output.txt; \
	done
	@rm -f benchmark_output.txt

clean:
	rm -f bin/$(PROGRAM)_*
	rm -f data/*