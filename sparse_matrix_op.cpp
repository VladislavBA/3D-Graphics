#include "stdio.h"
#include "math.h"
#include "pthread.h"
#include "multithread.h"
#include "sparse_matrix_op.h"

// Multithread
// M * y = f, M - preconditioner matrix (M = diag(A)^-1)
void multi_jacobi_preconditioner (const double *msr_matrix, const int n,          //< MSR-stored matrix, size n
                                  double *y, const double *f,                     //< vector and rhs
                                  const int my_rank, const int total_thread)      //< multithread data
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  for (int i = i1; i <= i2; i++)
    {
      y[i] = f[i] / msr_matrix[i];
    }

  synchronize (total_thread);
}

// Multithread
// A * v = r
void msr_multiply_matrix (const double *msr_matrix, const int* indexes, const int n, //< MSR-stored matrix, size n
                          const double *vector, double *result,                      //< vector and prec_vector
                          const int my_rank, const int total_thread)                 //< multithread data
{
  int len = 0, l = 0;
  double sum = 0;
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  for (int i = i1; i <= i2; i++)
    {
      sum = msr_matrix[i] * vector[i];  //< diagonal element
      len = indexes[i + 1] - indexes[i];
      l = indexes[i];

      for (int j = 0; j < len; j++)
        {
          sum += msr_matrix[l + j] * vector[indexes[l + j]];
        }
      result[i] = sum;
    }
  synchronize (total_thread);
}

double inner_product (const double *x, const double *y, const int n, double *sum,
                      const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;
  double sum_thread = 0;

  // residual of unroll loop
  for (int i = i1; i <= i2; i++)
    sum_thread += x[i] * y[i];

  sum[my_rank] = sum_thread;
  return reduce_sum (sum, my_rank, total_thread);
}
// y = y + scalar * z
void linear_combination (double *y, const double *z, const double scalar,
                         const int n, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  for (int i = i1; i <= i2; i++)
    y[i] += scalar * z[i];

  synchronize (total_thread);
}

void msr_residual (const double *msr_matrix, const int *indexes, const int n,
                   const double *x, const double *b, double *r, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  msr_multiply_matrix (msr_matrix, indexes, n, x, r, my_rank, total_thread);

  for (int i = i1; i <= i2; i++)
    {
      r[i] = b[i] - r[i];
    }

  synchronize (total_thread);
}

double compute_vector_inf_norm (const double *vector, const int n, double *max, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;
  double max_thread = 0.;

  for (int i = i1; i <= i2; i++)
    {
      if (max_thread < fabs (vector[i]))
        {
          max_thread = fabs (vector[i]);
        }
    }
  max[my_rank] = max_thread;
  return reduce_max_sum (max, my_rank, total_thread);
}

void copy (const double *src, double *dst, const int n, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  for (int i = i1; i <= i2; i++)
    dst[i] = src[i];
}
