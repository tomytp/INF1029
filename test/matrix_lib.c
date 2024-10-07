#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <immintrin.h>
#include "matrix_lib.h"

#define VEC_STEP 8
#define NUM_THREADS 8  

typedef struct {
    Matrix *matrixA;
    Matrix *matrixB;
    Matrix *matrixC;
    unsigned long start_row;
    unsigned long end_row;
} ThreadData;

int isValidMatrix(Matrix *matrix)
{
    return (matrix != NULL && matrix->rows != NULL && matrix->height % 8 == 0 && matrix->width % 8 == 0);
}

void *scalar_mult_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    Matrix *matrixA = data->matrixA;
    unsigned long start_row = data->start_row;
    unsigned long end_row = data->end_row;

    unsigned long matrix_width = matrixA->width;
    unsigned long start_index = start_row * matrix_width;
    unsigned long end_index = end_row * matrix_width;

    __m256 scalar, vec, result;
    float scalar_value = 5.0f; 

    scalar = _mm256_set1_ps(scalar_value);

    float *vec_next = &matrixA->rows[start_index];
    for (unsigned long i = start_index; i < end_index; i += VEC_STEP, vec_next += VEC_STEP) {
        vec = _mm256_load_ps(vec_next);
        result = _mm256_mul_ps(scalar, vec);
        _mm256_store_ps(vec_next, result);
    }

    pthread_exit(NULL);
}

int scalar_matrix_mult(float scalar_value, Matrix* matrix)
{
    if (!isValidMatrix(matrix)) {
        return 0;
    }

    unsigned long matrix_height = matrix->height;
    unsigned long rows_per_thread = matrix_height / NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].matrixA = matrix;  
        thread_data[t].matrixB = NULL;    
        thread_data[t].matrixC = NULL;    
        thread_data[t].start_row = t * rows_per_thread;

        if (t == NUM_THREADS - 1) {
            thread_data[t].end_row = matrix_height; 
        } else {
            thread_data[t].end_row = (t + 1) * rows_per_thread;
        }

        pthread_create(&threads[t], NULL, scalar_mult_thread, (void *)&thread_data[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    return 1;
}

void *matrix_mult_thread(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    Matrix *matrixA = data->matrixA;
    Matrix *matrixB = data->matrixB;
    Matrix *matrixC = data->matrixC;
    unsigned long start_row = data->start_row;
    unsigned long end_row = data->end_row;

    unsigned long int a_width = matrixA->width;
    unsigned long int b_width = matrixB->width;

    float *a_row = &matrixA->rows[start_row * a_width];
    float *c_row = &matrixC->rows[start_row * b_width];

    for (unsigned long int i = start_row; i < end_row; i++)
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

    pthread_exit(NULL);
}

int matrix_matrix_mult(Matrix *matrixA, Matrix *matrixB, Matrix *matrixC)
{
    if (!isValidMatrix(matrixA) || !isValidMatrix(matrixB) || !isValidMatrix(matrixC) ||
        matrixA->width != matrixB->height ||
        matrixC->height != matrixA->height ||
        matrixC->width != matrixB->width) {
        return 0;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    unsigned long rows_per_thread = matrixA->height / NUM_THREADS;

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].matrixA = matrixA;
        thread_data[t].matrixB = matrixB;
        thread_data[t].matrixC = matrixC;
        thread_data[t].start_row = t * rows_per_thread;
        if (t == NUM_THREADS - 1) {
            thread_data[t].end_row = matrixA->height;  
        } else {
            thread_data[t].end_row = (t + 1) * rows_per_thread;
        }

        pthread_create(&threads[t], NULL, matrix_mult_thread, (void *)&thread_data[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    return 1;
}
