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

int matrix_matrix_mult(Matrix* matrixA, Matrix* matrixB, Matrix* matrixC){
    if (!isValidMatrix(matrixA) || !isValidMatrix(matrixB) || !isValidMatrix(matrixC) ||
        matrixA->width != matrixB->height || 
        matrixC->height != matrixA->height || 
        matrixC->width != matrixB->width) {
        return 0;
    }

    unsigned long int a_height = matrixA->height;
    unsigned long int a_width = matrixA->width;
    unsigned long int b_width = matrixB->width;

    float *a_row = matrixA->rows, *c_row = matrixC->rows;
    for (unsigned long int i = 0; i < a_height; i++) {
        float *b_row = matrixB->rows;

        for (unsigned long int k = 0; k < a_width; k++) {
            float a_ik = a_row[k];
            
            for (unsigned long int j = 0; j < b_width; j++) {
                c_row[j] += a_ik * (*b_row++);
            }
        }
        
        a_row += a_width;
        c_row += b_width;
    }

    return 1;
}
