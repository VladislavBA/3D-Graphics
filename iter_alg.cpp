#include "stdio.h"
#include "math.h"

#include "thread_source.h"
#include "iter_alg.h"
#include "io.h"
#include "sparse_matrix_op.h"
#include "matrix_args.h"
#include "multithread.h"

static double get_residual (const double *msr_matrix, const int *indexes, const int n,
                            const double *x, const double *rhs, double *r, double *max,
                            const int my_rank, const int total_thread)
{
  msr_residual (msr_matrix, indexes, n, x, rhs, r, my_rank, total_thread);

  return compute_vector_inf_norm (r, n, max, my_rank, total_thread);
}

int minimal_residual_solver (const double *msr_matrix, const int *indexes, const int n,
                             const double *b, double *x, double *workspace,
                             const int my_rank, const int total_thread, const int maxit, const double eps)
{
  double *r = workspace; // residual vector
  double *v = r + n;     // additional vector v = Au
  // r = b - Ax
  double *u = v + n;     // precond vector u = M^(-1)r
  double *max = u + n;   // additional vector for compute
  int iter = 0;          // iteration
  double c1 = 0, c2 = 0, tau = 0;  // c1 = (Au, r)
                                   // c2 = (Au, Au)

  msr_residual (msr_matrix, indexes, n, x, b, r, my_rank, total_thread);

  for (iter = 0; iter < maxit; iter++)
    {
      // Mu = r
      multi_jacobi_preconditioner (msr_matrix, n, u, r, my_rank, total_thread);

      // v = Au
      msr_multiply_matrix (msr_matrix, indexes, n, u, v, my_rank, total_thread);

      // c1 = (v, r)
      c1 = inner_product (v, r, n, max, my_rank, total_thread);

      // c2 = (v, v)
      c2 = inner_product (v, v, n, max, my_rank, total_thread);

      if (c1 < eps * eps || c2 < eps * eps)
        break;

      // tau = (v, r) / (v, v) = c1 / c2
      tau = c1 / c2;

      // x += tau * u
      linear_combination (x, u, tau, n, my_rank, total_thread);

      // r += -tau * v
      linear_combination (r, v, -tau, n, my_rank, total_thread);

      double residual = compute_vector_inf_norm (r, n, max, my_rank, total_thread);
      if (my_rank == 0)
        {
          printf ("It = %d          Residual = %2.8e\n", iter, residual);
        }

      if (residual < eps * eps)
        break;
    }

  synchronize (total_thread);

  // iteration
  if (c1 < 0 || c2 < 0 || iter >= maxit)
    return -1;
  else
    return iter;

}

/*
static void Print_start_data (const double *msr_matrix, const int *indexes, const int n, const int nz, const double *rhs, const int my_rank)
{
  // Print matrix as arrays MSR ans INDEX
  print_msr_NF (msr_matrix, indexes, nz, my_rank);
  // Print rhs
  printf ("rhs = ");
  print_vector (rhs, n, my_rank);
}
*/
void *msr_solver (void* args)
{
  // class matrix arguments
  matrix_args *m_args = (matrix_args*)args;
  int my_rank = m_args->get_my_rank ();
  int total_thread = m_args->get_total_thread ();

  // iteration definition
  constexpr int maxit = 50;
  constexpr int globalmaxit = 300;
  constexpr double eps = 1e-4;
  int iter = 0;
  int glob_iter = 0;

  double *msr_matrix = m_args->get_matrix ();            //< MSR matrix
  double *x = m_args->get_x ();                 //< solve
  double *rhs = m_args->get_rhs ();             //< right hand
  double *workspace = m_args->get_workspace (); //< addition workspace
  double *r = workspace;                        //< residual vector
  int *indexes = m_args->get_indexes ();        //< MSR IND vector
  const int n = m_args->get_size ();            //< size matrix
  const int width = m_args->get_width ();       //< widht of band matrix
  double *max = r + 3 * n;
  double residual = 0;                          // residual
  double full_time = 0;

  // create msr matrix
  create_msr_band_matrix (msr_matrix, indexes, width, n, my_rank, total_thread);

  print_msr_matrix_SF (msr_matrix, indexes, n, my_rank);

  print_msr_NF (msr_matrix, indexes, m_args->get_nozero (), my_rank);

  // symmetry matrix
  /*  int sym = 0;
  sym = is_symmetric_msr_matrix (indexes, n, my_rank);
  if (sym)
    {
      printf ("MSR not symmtetry!!!\n");
    }
*/
  // set rhs = (1,0,1,0...)
  set_rhs_vector (msr_matrix, indexes,rhs, n, my_rank, total_thread);

  // set x = (1,0,1,0...)
  set_init_solve_vector (x, n, my_rank, total_thread);

  //  Print_start_data (msr_matrix, indexes, n, nz, rhs, my_rank);

  full_time = get_full_time (); // Initialize full time

  m_args->set_thread_time (get_time ()); // Initialize time

  for (glob_iter = 0; glob_iter < globalmaxit;)
    {
      iter = minimal_residual_solver (msr_matrix, indexes, n, rhs, x, workspace,
                                      my_rank, total_thread, maxit, eps);

      if (iter < 0)
        {
          glob_iter += maxit;
          residual = get_residual (msr_matrix, indexes, n, x, rhs, r, max, my_rank, total_thread);
          if (my_rank == 0)
            printf("Failure:        iters = %d         residual = %4.8e\n", glob_iter, residual);
        }
      else
        break;
    }
  // Finish time
  full_time = get_full_time () - full_time;
  m_args->set_thread_time(get_time () - m_args->get_thread_time ());

  residual = get_residual (msr_matrix, indexes, n, x, rhs, r, max, my_rank, total_thread);

  if (my_rank == 0)
    {
      double error = 0;
      printf ("MATRIX: %s\nResidual  =  %e        Error  =  %e\nFull_time  =   %f\n", "no-file", residual, error, full_time);
    }

  printf ("Thread   # %d  ------------------- CPU_thread_time = %f\n", my_rank, m_args->get_thread_time ());
  synchronize (total_thread);
  return 0;
}
