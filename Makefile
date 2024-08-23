CC = gcc
PROGRAM = test
HEIGHT = 16
WIDTH = 16

all: run

.PHONY: run build 

build: src/matrix_lib.c src/timer.c src/matrix_lib_test.c
	$(CC) $(CFLAGS) -o bin/$(PROGRAM) src/matrix_lib.c src/timer.c src/matrix_lib_test.c

run:
	python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_a.puc -v 2
	python3 input_generator.py $(HEIGHT) $(WIDTH) data/input_b.puc -v 5
	./$(PROGRAM) 5.0 $(HEIGHT) $(WIDTH) $(HEIGHT) $(WIDTH) data/input_a.puc data/input_b.puc data/output_1.puc data/output_2.puc
