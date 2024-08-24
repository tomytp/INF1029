#include <stdio.h>
#include <stdlib.h>
#include "matrix_lib.h"
#include "timer.h"

void read_matrix_from_file(Matrix *mat, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }

    fread(mat->rows, sizeof(float), mat->height * mat->width, file);
    fclose(file);
}

void write_matrix_to_file(Matrix *mat, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(1);
    }

    fwrite(mat->rows, sizeof(float), mat->height * mat->width, file);
    fclose(file);
}

Matrix *initialize_Matrix(unsigned long int height, unsigned long int width) {
    Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));
    matrix->height = height;
    matrix->width = width;
    matrix->rows = (float *) malloc(height * width * sizeof(float));
    if (matrix->rows == NULL) {
        printf("Allocation error.\n");
        exit(1);
    }

    return matrix;
}

void free_Matrix(Matrix *matrix) {
    free(matrix->rows);
    free(matrix);
}

void print_matrix(struct matrix *mat) {
    for (unsigned long int i = 0; i < mat->height; i++) {
        for (unsigned long int j = 0; j < mat->width; j++) {
            printf("%.2f ", mat->rows[i * mat->width + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 10) {
        printf("Usage: %s <scalar> <heightA> <widthA> <hightB> <widthB> <inputA> <inputB> <outputA> <outputB>\n", argv[0]);
        return 1;
    }

    float scalar = atof(argv[1]);
    unsigned long int heightA = strtoul(argv[2], NULL, 10);
    unsigned long int widthA = strtoul(argv[3], NULL, 10);
    unsigned long int heightB = strtoul(argv[4], NULL, 10);
    unsigned long int widthB = strtoul(argv[5], NULL, 10);

    if (widthA != heightB) {
        printf("Incompatible Matrixes (widthA != heightB).\n");
        return 1;
    }

    Matrix *matrixA, *matrixB, *matrixC;
    matrixA = initialize_Matrix(heightA, widthA);
    matrixB = initialize_Matrix(heightB, widthB);
    matrixC = initialize_Matrix(heightA, widthB);

    read_matrix_from_file(matrixA, argv[6]);
    read_matrix_from_file(matrixB, argv[7]);

    struct timeval function_start, function_stop, overall_start, overall_stop;

    gettimeofday(&overall_start, NULL);

    // print_matrix(matrixA);

    gettimeofday(&function_start, NULL);
    if (!scalar_matrix_mult(scalar, matrixA)) {
        printf("Failed to multiply matrixA by scalar.");
        return 1;
    }
    gettimeofday(&function_stop, NULL);
    printf("Scalar Mult duration: %f ms\n", timedifference_msec(function_start, function_stop));
    write_matrix_to_file(matrixA, argv[8]);
    // print_matrix(matrixA);


    gettimeofday(&function_start, NULL);
    if (!matrix_matrix_mult(matrixA, matrixB, matrixC)) {
        printf("Failed to multiply matrixA by matrixB.");
        return 1;
    }
    gettimeofday(&function_stop, NULL);
    printf("Matrix Mult duration: %f ms\n", timedifference_msec(function_start, function_stop));
    write_matrix_to_file(matrixC, argv[9]);
    // print_matrix(matrixC);

    gettimeofday(&overall_stop, NULL);
    printf("Total duration: %f ms\n", timedifference_msec(overall_start, overall_stop));

    free_Matrix(matrixA);
    free_Matrix(matrixB);
    free_Matrix(matrixC);

    return 0;
}
