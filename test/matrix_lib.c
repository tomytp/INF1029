#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "matrix_lib.h"

#define VEC_STEP 8

int isValidMatrix(Matrix *matrix)
{
    return (matrix != NULL && matrix->rows != NULL && matrix->height % 8 == 0 && matrix->width % 8 == 0);
}

int scalar_matrix_mult(float scalar_value, Matrix* matrix){
    if (!isValidMatrix(matrix)) {
        return 0;
    }

    __m256 scalar, vec, result;
    float * vec_next = matrix->rows;
    unsigned long int matrix_size = matrix->height * matrix->width;
    scalar = _mm256_set1_ps(scalar_value);

    for(int i = 0; i < matrix_size; i += VEC_STEP, vec_next += VEC_STEP){
        vec = _mm256_load_ps(vec_next);
        result = _mm256_mul_ps(scalar, vec);
        _mm256_store_ps(vec_next, result);
    }
    return 1;
}

int matrix_matrix_mult(Matrix *matrixA, Matrix *matrixB, Matrix *matrixC)
{
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
    for (unsigned long int i = 0; i < a_height; i++)
    {
        for (unsigned long int k = 0; k < a_width; k++)
        {
            float a_ik = a_row[k];
            __m256 scalar = _mm256_set1_ps(a_ik);

            float *b_row = &matrixB->rows[k * b_width];

            for (unsigned long int j = 0; j < b_width; j += VEC_STEP)
            {
                __m256 vec_b = _mm256_load_ps(&b_row[j]);
                __m256 vec_c = _mm256_load_ps(&c_row[j]);
                vec_c = _mm256_fmadd_ps(scalar, vec_b, vec_c);
                _mm256_store_ps(&c_row[j], vec_c);
            }
        }

        a_row += a_width;
        c_row += b_width;
    }

    return 1;
}
