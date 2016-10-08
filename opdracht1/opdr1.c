void print_roman(int x);

#define N 16

static int A[N], B[N];

int main(void)
{
    int C, i;

    for (i = 0; i < N; ++i)
      A[i] = i * 2;

    C = 95;

    B[0] = 0;
    for (i = 1; i < N; ++i)
      B[i] = A[i-1] + C;

    print_roman(B[N-1]);

    return 0;
}
