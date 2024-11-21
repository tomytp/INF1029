CC = gcc
CFLAGS = -std=c11 -mfma -pthread
PROGRAM = test
HEIGHT = 512
WIDTH = 512
SCALAR = 5.0
THREAD_COUNT = 8
LIB = matrix_lib_amx_thread

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

.PHONY: run build_all clean

build: bin/$(PROGRAM)_$(LIB)

bin/$(PROGRAM)_%: src/%.c src/timer.c src/matrix_lib_test.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

1024:
	$(MAKE) run HEIGHT=1024 WIDTH=1024

2048:
	$(MAKE) run HEIGHT=2048 WIDTH=2048

run:
	@mkdir -p data
	@if [ ! -f $(INPUT_A) ]; then \
	    python input_generator.py $(HEIGHT) $(WIDTH) $(INPUT_A) -v 2; \
	fi	
	@if [ ! -f $(INPUT_B) ]; then \
	    python input_generator.py $(HEIGHT) $(WIDTH) $(INPUT_B) -v 5; \
	fi

	@echo "Running for size=$(HEIGHT)x$(WIDTH) / algorithm=$(LIB)"
	./bin/$(PROGRAM)_$(LIB) $(SCALAR) $(HEIGHT) $(WIDTH) $(HEIGHT) $(WIDTH) $(THREAD_COUNT) $(INPUT_A) $(INPUT_B) $(OUTPUT_1) $(OUTPUT_2)
	@python check_results.py $(HEIGHT) $(WIDTH) $(SCALAR) $(INPUT_A) $(INPUT_B) $(OUTPUT_2) $(LIB)

clean:
	rm -f bin/$(PROGRAM)_*
	rm -f data/*