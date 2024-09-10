#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix_lib.h"
#include "timer.h"

int load_matrix(Matrix *mat, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        return 0;
    }

    fread(mat->rows, sizeof(float), mat->height * mat->width, file);
    fclose(file);
    return 1;
}

int store_matrix(Matrix *mat, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        return 0;
    }

    fwrite(mat->rows, sizeof(float), mat->height * mat->width, file);
    fclose(file);
    return 1;
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

void print_matrix(Matrix* matrix){
    int cont = 0;
    for(int i = 0; i < matrix->height; i++){
        for(int j = 0; j < matrix->width; j++){
            if(cont == 256){
                printf("\nOutput too large, printing only 256 values\n");
                return;
            }
            printf("%.2f ", matrix->rows[i * matrix->width + j]);
            cont++;
        }
        printf("\n");
    }
}


int check_errors(struct matrix *matrix, float scalar_value) {
    if (matrix == NULL || matrix->rows == NULL) {
        return -1;
    }

    float max_difference = 0;
    unsigned long int total_elements = matrix->height * matrix->width;

    for (unsigned long int i = 0; i < total_elements; i++) {
        float difference = fabsf(matrix->rows[i] - scalar_value);
        if (difference > max_difference) {
            max_difference = difference;
        }
    }

    return max_difference;
}

int main(int argc, char *argv[]) {
    if (argc != 10) {
        printf("Usage: %s <scalar> <heightA> <widthA> <hightB> <widthB> <inputA> <inputB> <outputA> <outputB>\n", argv[0]);
        return 1;
    }

    struct timeval function_start, function_stop, overall_start, overall_stop;
    gettimeofday(&overall_start, NULL);
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

    load_matrix(matrixA, argv[6]);
    load_matrix(matrixB, argv[7]);
    memset(matrixC->rows, 0, matrixC->height * matrixC->width * sizeof(float));


    printf("Executing scalar_matrix_mult(%5.1f, matrixA)...\n", scalar);
    gettimeofday(&function_start, NULL);
    if (!scalar_matrix_mult(scalar, matrixA))
    {
        printf("%s: scalar_matrix_mult problem.", argv[0]);
        return 1;
    }
    gettimeofday(&function_stop, NULL);
    printf("%f ms\n", timedifference_msec(overall_start, function_stop));

    /* Print matrix */
    printf("---------- Matrix A ----------\n");
    print_matrix(matrixA);

    /* Write first result */
    printf("Writing first result: %s...\n", argv[8]);
    if (!store_matrix(matrixA, argv[8]))
    {
        printf("%s: failed to write first result to file.", argv[0]);
        return 1;
    }

    /* Calculate the product between matrix A and matrix B */
    printf("Executing matrix_matrix_mult(matrixA, mattrixB, matrixC)...\n");
    gettimeofday(&function_start, NULL);
    if (!matrix_matrix_mult(matrixA, matrixB, matrixC))
    {
        printf("%s: matrix_matrix_mult problem.", argv[0]);
        return 1;
    }
    gettimeofday(&function_stop, NULL);
    printf("%f ms\n", timedifference_msec(function_start, function_stop));

    /* Print matrix */
    printf("---------- Matrix C ----------\n");
    print_matrix(matrixC);

    /* Write second result */
    printf("Writing second result: %s...\n", argv[9]);
    if (!store_matrix(matrixC, argv[9]))
    {
        printf("%s: failed to write second result to file.", argv[0]);
        return 1;
    }

    /* Check foor errors */
    printf("Checking matrixC for errors...\n");
    gettimeofday(&function_start, NULL);
    float max_error = check_errors(matrixC, matrixC->rows[0]);
    gettimeofday(&function_stop, NULL);
    printf("%f ms\n", timedifference_msec(function_start, function_stop));
    if (max_error != -1) {
        printf("Max error: %f\n", max_error);
    }

    // Mark overall stop time
    gettimeofday(&overall_stop, NULL);

    // Show elapsed overall time
    printf("Overall time: %f ms\n", timedifference_msec(overall_start, overall_stop));
    free_Matrix(matrixA);
    free_Matrix(matrixB);
    free_Matrix(matrixC);

    return 0;
}
