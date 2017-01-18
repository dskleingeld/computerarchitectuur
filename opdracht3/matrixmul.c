#include <immintrin.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <math.h>


/* Size of the matrix. We use N x N square matrices. */
#define N 2048


static void
matrix_clear(float matrix[N][N])
{
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      matrix[i][j] = 0;
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
        printf("%f ", matrix[i][j]);
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
multiply(float C[N][N], const float A[N][N], const float B[N][N])
{
  for (size_t i = 0; i < N; ++i)
    for (size_t j = 0; j < N; ++j)
      {
        float row = 0.0;
        for (size_t k = 0; k < N; k++)
          row += A[i][k] * B[k][j];
        C[i][j] = row;
      }
}


int
main(int argc, char **argv)
{
  /* We use this "magic" to allocate real two-dimensional arrays, that
   * are aligned at 32-bytes. This is a requirement for AVX.
   */
  float (*A)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);
  float (*B)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);

  matrix_init_identity(A);
  matrix_init_random(B);

  float (*C)[N] = (float (*)[N])_mm_malloc(N * N * sizeof(float), 32);

  multiply(C, A, B);

  _mm_free(A);
  _mm_free(B);
  _mm_free(C);

  return 0;
}
