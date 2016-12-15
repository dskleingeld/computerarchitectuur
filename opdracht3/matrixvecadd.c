#include <immintrin.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>


/* Size of the matrix. We use N x N square matrices. */
#define N 1024


static void
matrix_clear(float matrix[N][N])
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      matrix[i][j] = 0;
}

static void
matrix_copy(float dst[N][N], const float src[N][N])
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      dst[i][j] = src[i][j];
}

static void
matrix_init_identity(float matrix[N][N])
{
  matrix_clear(matrix);

  for (int i = 0; i < N; ++i)
    matrix[i][i] = 1.;
}

static void
matrix_init_random(float matrix[N][N])
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      matrix[i][j] = rand() % 100;
}

static void
matrix_dump(float matrix[N][N])
{
  for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
        printf("%f ", matrix[j][j]);
      printf("\n");
    }
}

static bool
matrix_equal(float A[N][N], float B[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      if (fabsf(A[i][j] - B[i][j]) > 0.000001)
        return false;

  return true;
}

static void
vector_init_counting(float b[N])
{
  for (size_t i = 0; i < N; ++i)
    b[i] = i;
}


static void
add_vec(float A[N][N], const float b[N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      A[j][i] = A[j][i] * 2. + b[j];
}


int
main(int argc, char **argv)
{
  /* We use this "magic" to allocate real two-dimensional arrays, that
   * are aligned at 32-bytes. This is a requirement for AVX.
   */
  float (*A)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);
  float *b = (float *)_mm_malloc(N * sizeof(float), 32);

  matrix_init_random(A);
  vector_init_counting(b);

  /* Repeat the experiment 1000 times to get stable measurements. */
  for (int i = 0; i < 1000; ++i)
    add_vec(A, b);

  _mm_free(A);
  _mm_free(b);

  return 0;
}
