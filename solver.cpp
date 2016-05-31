#include "solver.h"
#include "matrix_args.h"

solver::solver (const int argc, const char *argv[])
{
  m_error = parser (argc, argv);

  m_msr_matrix = nullptr;
  m_indexes = nullptr;
  m_solve = nullptr;
  m_rhs = nullptr;
  m_workspace = nullptr;

  m_args = nullptr;

  m_fill_matrix_handler = nullptr;

  m_size = 0;
  m_nozero = 0;
  m_work_len = 0;
  m_all_time = 0.;
  m_CPU_time = 0.;
  m_tid = 0;
}

bool solver::is_error ()
{
  if (m_error < 0)
    return true;

  return false;
}

int solver::parser (const int argc, const char *argv[])
{
  double a, b, c, d, p, q;
  int m, n, total_thread;

  if (argc < 10)
    {
      printf("A few args\n");
      return -1;
    }
  if ((sscanf (argv[1], "%lf", &a) != 1) || (sscanf (argv[2], "%lf", &b) != 1) ||
      (sscanf (argv[3], "%lf", &c) != 1) || (sscanf (argv[4], "%lf", &d) != 1) ||
      (sscanf (argv[5], "%lf", &p) != 1) || (sscanf (argv[6], "%lf", &q) != 1) ||
      (sscanf (argv[7], "%lf", &m) != 1) || (sscanf (argv[8], "%lf", &n) != 1) ||
      (sscanf (argv[9], "%lf", &total_thread) != 1)
     )
    {
      printf("Cannot read args!\n");
      return -2;
    }
  m_input_data = {a, b, c, d, p, q, m, n, total_thread};

  return 0;
}

int solver::start_solver ()
{
  m_args = new matrix_args [m_input_data.total_thread]();

  // build pattern???

  for (int i = 1; i < total_thread; i++)
    {
      if (pthread_create (&m_tid, 0, multithread_msr_solver, args + i))
        {
          printf ("Cannot create thread %d\n", i);
          return 100;
        }
    }

  m_fill_matrix_handler = new fill_in_matrix ();

  m_args = new matrix_args [m_input_data.total_thread]();
  for (int i = 0; i < m_input_data.total_thread; i++)
    {
      m_args[i].matrix_args_init (m_msr_matrix, m_indexes, m_solve, m_rhs,
                                  m_size, m_nozero, i, m_input_data.total_thread,
                                  m_workspace, m_work_len);
    }
}

void *solver::multithread_msr_solver (void* args)
{
  matrix_args *thread_args = static_cast<matrix_args *> (args);
  double *msr_matrix = nullptr;
  double *

  while (/*SIGNAL OF END*/)
    {
      args[my_rank].set_time(get_timeofday);
      // fillinmatrix data

      // set_memmory

      // set arguments

      minimal_residual_solver ()

      synchronize (total_thread);

      send_ready_solve();

      get_singal ();
    }

  return;
}
