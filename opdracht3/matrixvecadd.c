#include <immintrin.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
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


static void
add_vec2(float A[N][N], const float b[N])
{
	//find a suitable block size:
	///sys/devices/system/cpu/cpu0/cache/index3 (cache info)
	//L1 size: 32K
	//L2 size: 256K
	//L3 size: 6144K
	
	//calculation we perform to find blocksize then input below
	//const static in cacheLineSize = 32 //in KB (kilo byte)
	//const static int blockSize = 2^(int)log(sqrt(cacheLineSize*1000))/log(2)
	const static int blockSize = 128;
	
	for(int i=0; i<N; i+=blockSize)
		for(int j=0; j<N; j+=blockSize)
			for (size_t x = i; x < i+blockSize; ++x)
				for (size_t y = j; y < j+blockSize; ++y)
				  A[y][x] = A[y][x] * 2. + b[y];
}

//note arrays are stored in memory like A[i][:] the i's after eachother
static void
add_vec3(float A[N][N], const float b[N]) {
	float bj;

	float mul = 2.;
	__m256 D = _mm256_broadcast_ss(&mul); //store multiplier

	__m256 C; //store a subrow of array A
	__m256 E; //store the b value for addition
	
	const static int blockSize = 8;
  for (size_t j = 0; j < N; ++j){//for columns
  	E = _mm256_broadcast_ss(&b[j]);
		for(int i=0; i<N; i+=blockSize){
			C = _mm256_load_ps(&A[j][i]); //load 8 floats into a vector A
			C = _mm256_mul_ps(C, D);
			
			C = _mm256_add_ps(C, E);
			
			_mm256_store_ps(&A[j][i], C);
	  }
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

  float *b = (float *)_mm_malloc(N * sizeof(float), 32);

  matrix_init_random(A);
  vector_init_counting(b);

	//matrix_copy(B, A);
	//add_vec(A, b);
	//add_vec3(B, b);
	
	//if(matrix_equal(A,B)){printf("equal");}
	
  /* Repeat the experiment 1000 times to get stable measurements. */
  for (int i = 0; i < 1000; ++i)
    add_vec3(A, b);
		

  _mm_free(A);
  _mm_free(b);

  return 0;
}
