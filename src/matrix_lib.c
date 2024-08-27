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

    int imult = 0;
    for (unsigned long int i = 0; i < matrixA->height; i++) {
        for (unsigned long int j = 0; j < matrixB->width; j++) {
            int ij = imult + j;
            int kmult = 0;
            for (unsigned long int k = 0; k < matrixA->width; k++) {
                matrixC->rows[ij] += matrixA->rows[imult + k] * matrixB->rows[kmult + j];
                kmult += matrixB->width;
            }
        }
        imult += matrixA->width;
    }

    return 1;
}

