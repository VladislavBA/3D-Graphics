#include "multithread.h"
#include "io.h"
#include "stdio.h"
#include "pthread.h"


int is_symmetric_msr_matrix (const int *indexes, const int n, const int my_rank)
{
  if (my_rank > 0)
    return 0;
  int col_num = 0;

  // loop for all upperdiag element matrix
  for (int row_num = 0; row_num < n; row_num++)
    {
      // the number of entry of current row
      int len = indexes[row_num + 1] - indexes[row_num];

      // loop for elements of current row
      for (int j = 0; j < len; j++)
        {
          // the number of colomn current element
          col_num = indexes[indexes[row_num] + j];
          // if element is placed lower or no diag then continue next element
          if (col_num <= row_num)
            continue;
          // the number of element of searching row
          int len_col = indexes[col_num + 1] - indexes[col_num];
          int i = 0;
          // search symmetric element
          for (i = 0; i < len_col; i++)
            {
              // if exist symmetric then go out loop
              if (indexes[indexes[col_num] + i] == row_num)
                break;
            }
          // if we haven't find element then nonsymmetry
          if (i == len_col)
            return -1;
        }
    }
  return 0;
}
// What the format can we use???
/*
int read_msr_matrix_from_file (double *msr_matrix, int *indexes, const int n, const char *file, const int my_rank, const int process_number)
{
    return 0;
}
*/

static int get_position_curr_row (const int row_num, const int width_band, const int n)
{
  if (n < 0 || width_band < 1 || row_num >= n || row_num <= 0)
    return 0;

  return (row_num - 1) * (width_band - 1) + (width_band - 1) / 2;
}

int create_msr_band_matrix (double *msr_matrix, int *indexes, const int width,
                            const int n, const int my_rank, const int total_thread)
{
  // compute the thread's boards
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;

  // diag = a(i,i) = - (a(i, 1) + ... + a(i, n))
  const double diag = static_cast<double> (width + 5);
  static int nz_elements_count = 0;
  int len = 0;
  int pos = 0;

  // create diag msr-matrix
  for (int i = i1; i <= i2; i++)
    msr_matrix[i] = diag;

  // build pattern for first nz element of row
  const int begin_pos = n + 1;
  for (int i = i1; i <= i2; i++)
    {
      pos = get_position_curr_row (i, width, n);
      indexes[i] = begin_pos + pos;
    }

    synchronize (total_thread);


  // set all the nz number on n position
  if (my_rank == total_thread - 1)
    {
      indexes[n] = indexes[n - 1] + (width - 1) / 2;
      nz_elements_count = indexes[n];
      msr_matrix[n] = 0.;
    }

  // fillin msr_matrix nz elements
  for (int i = i1; i <= i2; i++)
    {
      len = indexes[i + 1] - indexes[i];
      pos = indexes[i];
      for (int j = 0; j < len; j++)
        {
          // SEGF
          msr_matrix [pos + j] = -1;
          if (i == 0)
            {
              indexes [pos + j] = i + j + 1;
              continue;
            }
          if ((i - (width - 1) / 2 + j) < i)
            indexes [pos + j] = i - (width - 1) / 2 + j;
          else
            indexes [pos + j] = i - (width - 1) / 2 + j + 1;
        }
    }

  synchronize (total_thread);

  return nz_elements_count;
}

// vector sum element matrix
void set_rhs_vector (const double *msr_matrix, const int *indexes, double *rhs,
                     const int n, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;
  for (int i = i1; i <= i2; i++)
    {
      int len = indexes[i + 1] - indexes[i];
      rhs[i] = (i % 2 == 0) ? msr_matrix[i] : 0.;

      int i_row_pos = indexes[i];
      for (int j = 0; j < len; j++)
        {
          if (indexes[i_row_pos + j] % 2 == 0)
            rhs[i] += msr_matrix[i_row_pos + j];
        }
    }

  synchronize (total_thread);
}

void set_init_solve_vector (double *x, const int n, const int my_rank, const int total_thread)
{
  int i1 = my_rank * n;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n;
  i2 = i2 / total_thread - 1;
  for (int i = i1; i <= i2; i++)
    {
      x[i] = (i % 2) ? 0 : 5;
    }

  synchronize (total_thread);
}

void print_msr_NF (const double *msr_matrix, const int *indexes, const int nz, const int my_rank)
{
  if (my_rank != 0)
    return;
  static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock (&m);

  printf("MSR>>>>>>\n");
  for (int i = 0; i < nz; i++)
    {
      printf ("%2.2f  ", msr_matrix[i]);
      if ((i > 0) && (i % 10 == 0))
        printf ("\n");
    }
  printf ("<<<<<<<\n");
  printf("IND>>>>>>\n");
  for (int i = 0; i < nz; i++)
    {
      printf ("%d  ", indexes[i]);
      if ((i > 0) && (i % 10 == 0))
        printf ("\n");
    }
  printf ("<<<<<<<\n");
  pthread_mutex_unlock (&m);
}

void print_msr_matrix_SF (const double *msr_matrix, const int *indexes, const int n, const int my_rank)
{
  if (my_rank > 0)
    return;

  for (int row = 0; row < n; row++)
    {
      int end_nz = indexes[row + 1];
      int pos_nz = indexes[row];
      for (int col = 0; col < n; col++)
        {
          if (col != row)
            {
              if (indexes[pos_nz] == col)
                {
                  printf ("%2.4f  ", msr_matrix[pos_nz]);
                  if (pos_nz < end_nz) pos_nz++;
                }
              else
                {
                  printf("%2.4f   ", 0.0);
                }
            }
          else
            printf("%2.4f   ", msr_matrix[row]);
        }
      printf("\n");
    }
}

void print_vector (const double *vector, const int len)
{
  const int maxlen = 10;
  int n = (len > maxlen) ? maxlen : len;
  printf("vector>>>\n");
  for (int i = 0; i < n; i++)
    {
      printf("%2.2e  ", vector[i]);
    }
  printf("<<<<<\n");
}
