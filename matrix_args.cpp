#include "matrix_args.h"

matrix_args::matrix_args ()
{
  msr_matrix_ = nullptr;
  x_ = nullptr;
  b_ = nullptr;
  indexes_ = nullptr;
  n_ = 0;
  nz_ = 0;
  my_rank_ = 0;
  total_thread_ = 0;
  workspace_ = nullptr;
  work_len_ = 0;
  thread_time_ = 0;
}

matrix_args::~matrix_args ()
{
  msr_matrix_ = nullptr;
  indexes_ = nullptr;
  x_ = nullptr;
  b_ = nullptr;
  nz_ = 0;
  n_ = 0;
  my_rank_ = 0;
  total_thread_ = 0;
  workspace_ = nullptr;
  work_len_ = 0;
  thread_time_ = 0;
}

void matrix_args::matrix_args_init (double *matrix, int *ind, double *x, double *b, int n,
                                    int nz, int my_rank, int total_thread,
                                    double *workspace, int work_len)
{
  msr_matrix_ = matrix;
  indexes_ = ind;
  x_ = x;
  b_ = b;
  n_ = n;
  nz_ = nz;
  my_rank_ = my_rank;
  total_thread_ = total_thread;
  workspace_ = workspace;
  work_len_ = work_len;
}

int matrix_args::get_my_rank() const
{
  return my_rank_;
}

int matrix_args::get_total_thread() const
{
  return total_thread_;
}

double *matrix_args::get_matrix()
{
  return msr_matrix_;
}

double *matrix_args::get_x()
{
  return x_;
}

double *matrix_args::get_rhs()
{
  return b_;
}

int matrix_args::get_size() const
{
  return n_;
}

int *matrix_args::get_indexes()
{
  return indexes_;
}

int matrix_args::get_width() const
{
  return width_;
}

double *matrix_args::get_workspace()
{
  return workspace_;
}

int matrix_args::get_nozero() const
{
  return nz_;
}
double matrix_args::get_thread_time()
{
  return thread_time_;
}
void matrix_args::set_thread_time (const double current_time)
{
  thread_time_ = current_time;
}
