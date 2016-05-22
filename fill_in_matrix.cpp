#include "fill_in_matrix.h"

fill_in_matrix::pattern_1 (int *index, const int i, const int j, const int m, const int n)
{
    index[0] = get_l (i - 1, j, m, n);
    index[1] = get_l (i - 1, j + 1, m, n);
    index[2] = get_l (i, j - 1, m, n);
    index[3] = get_l (i, j + 1, m, n);
    index[4] = get_l (i + 1, j - 1, m, n);
    index[5] = get_l (i + 1, j, m, n);
}

fill_in_matrix::pattern_2 (int *index, const int i, const int j, const int m, const int n)
{
    if (i == 0)
    {
        index[0] = get_l (i, j + 1, m, n);
        index[1] = get_l (i + 1, j, m, n);
    }
    else
    {
        index[0] = get_l (i - 1, j, m, n);
        index[1] = get_l (i, j - 1, m, n);
    }
}

fill_in_matrix::pattern_3 (int *index, const int i, const int j, const int m, const int n)
{
    if (i == 0)
    {
        index[0] = get_l (i, j - 1, m, n);
        index[1] = get_l (i + 1, j - 1, m, n);
        index[2] = get_l (i + 1, j, m, n);
    }
    else
    {
        index[0] = get_l (i - 1, j, m, n);
        index[1] = get_l (i - 1, j + 1, m, n);
        index[2] = get_l (i, j + 1, m, n);
    }
}

fill_in_matrix::pattern_4 (int *index, const int i, const int j, const int m, const int n)
{
    if (i == 0)
    {
        index[0] = get_l (i, j - 1, m, n);
        index[1] = get_l (i, j + 1, m, n);
        index[2] = get_l (i + 1, j - 1, m, n);
        index[3] = get_l (i + 1, j, m, n);
    }
    if (j == 0)
    {
        index[0] = get_l (i - 1, j, m, n);
        index[1] = get_l (i - 1, j + 1, m, n);
        index[2] = get_l (i, j + 1, m, n);
        index[3] = get_l (i + 1, j, m, n);
    }
    if (i == m || i == cut_row_)
    {
        index[0] = get_l (i, j - 1, m, n);
        index[1] = get_l (i - 1, j, m, n);
        index[2] = get_l (i - 1, j + 1, m, n);
        index[3] = get_l (i, j + 1, m, n);
    }
    if (j == n || j == cut_col_)
    {
        index[0] = get_l (i - 1, j, m, n);
        index[1] = get_l (i + 1, j, m, n);
        index[2] = get_l (i + 1, j - 1, m, n);
        index[3] = get_l (i, j - 1, m, n);
    }
}

fill_in_matrix::fill_in_matrix (const int cut_row, const int cut_col, const double jacobian)
{
  cut_row_ = cut_row;
  cut_col_ = cut_col;
  J_ = jacobian;
}

fill_in_matrix::~fill_in_matrix () {}

int fill_in_matrix::get_nz_elements_current_node (const int i, const int j, const int row_cut, const int col_cut, const int m, const int n, int* index)
{
  if ((i > 0 && j > 0 && i < m && j < n && (i > row_cut && j < col_cut || i <= row_cut))) // FULL SUPP
  {
    pattern_1 (index, i, j, m, n);
    return 6;
  }
  if ((i == 0 && j == 0) || (i == row_cut && j == n) || (i == m && j == col_cut)) // LEFT UP || RIGHT_DOWN_CUT || DOWN_RIGHT_CUT
  {
    pattern_2 (index, i, j, m, n);
    return 2;
  }
  if ((i == m && j == 0) || (i == 0 && j == n)) // LEFT_DOWN || RIGHT_UP
  {
    pattern_3 (index, i, j, m, n);
    return 3;
  }
  pattern_4 (index, i, j, m, n);
  return 4;
}

int fill_in_matrix::get_nozero_elements (const int m, const int n, const int my_rank, const int total_thread)
{
    int i1 = my_rank * m;
    i1 /= total_thread;
    int i2 = (my_rank + 1) * m;
    i2 = i2 / p - 1;

    int nz = 0;

    for (int i = i1; i <= i2; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            nz += get_nz_elements_current_node (i, j, cut_row_, cut_col_, m, n);
        }
    }
    reduce_sum (&nz, 1, total_thread);
    return nz;
}

int fill_in_matrix::fill_indexes (const int m, const int n, int *indexes)
{
    constexpr int max_row_nz = 6;
    int ind_loc[max_row_nz];

    int global_index = 0;
    int NZ = m * n;
    int nz = 0;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++, global_index++)
        {
          nz = get_nz_elements_current_node (i, j, cut_row_, cut_col_, m, n, ind_loc);
          indexes[global_index] = NZ;
          for (int s = 0; s < nz; s++)
          {
              indexes[NZ + s] = ind_loc[s];
          }
          NZ += nz;
        }
      indexes[global_index] = NZ;
    }
    return NZ;
}

void fill_in_matrix::fillin_msr_matrix (double *msr_matrix, const int *indexes, const int m, const int n, const int nz, const int my_rank, const int total_thread)
{
  const int N = n * m;
  int i1 = my_rank * N;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * N;
  i2 = i2 / total_thread - 1;
  const double I0 = (1. / 2) * J_;
  const double I1 = (1. / 12) * J_;
  int nz = 0;
  int begin = 0;

  for (int i = i1; i <= i2; i++)
  {
    msr_matrix[i] = I0;
    nz = indexes[i + 1] - indexes[i];
    begin = indexes[i];
    for (int j = 0; j < nz; j++)
    {
      msr_matrix[begin + j] = I1;
    }
  }
}


