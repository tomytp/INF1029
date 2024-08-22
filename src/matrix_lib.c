#include <stdio.h>
#include <stdlib.h>
#include "matrix_lib.h"

int isValidMatrix(Matrix *matrix) {
    return(matrix != NULL && matrix->rows != NULL && matrix->height % 8 == 0 && matrix->width % 8 == 0);
}

int scalar_matrix_mult(float scalar_value, Matrix *matrix) {
    if (!isValidMatrix(matrix)) {
        return 0;
    }

    unsigned long int total_elements = matrix->height * matrix->width;
    for (unsigned long int i = 0; i < total_elements; i++) {
        matrix->rows[i] *= scalar_value;
    }
    
    return 1;
}

int matrix_matrix_mult(Matrix *matrixA, Matrix *matrixB, Matrix *matrixC) {
    if (!isValidMatrix(matrixA) || !isValidMatrix(matrixB) || !isValidMatrix(matrixC) ||
        matrixA->width != matrixB->height || 
        matrixC->height != matrixA->height || 
        matrixC->width != matrixB->width) {
        return 0;
    }

    for (unsigned long int i = 0; i < matrixA->height; i++) {
        for (unsigned long int j = 0; j < matrixB->width; j++) {
            matrixC->rows[i * matrixC->width + j] = 0;
            for (unsigned long int k = 0; k < matrixA->width; k++) {
                matrixC->rows[i * matrixC->width + j] += matrixA->rows[i * matrixA->width + k] * matrixB->rows[k * matrixB->width + j];
            }
        }
    }

    return 1;
}
