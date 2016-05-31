#ifndef SOLVER_H
#define SOLVER_H

#include "multithread.h"

class matrix_args;
class fill_in_matrix;

struct input_data
{
  // rectangular
  double left_down_point_a;
  double right_down_point_b;
  double left_down_point_c;
  double left_up_point_d;

  // cut rectangular
  double left_down_cut_p;
  double right_up_cut_q;

  size_t num_point_mesh_m;
  size_t num_point_mesh_n;

  int total_thread;
};

class solver
{
private:

    input_data m_input_data;
    double *m_msr_matrix;
    int *m_indexes;
    double *m_solve;
    double *m_rhs;
    size_t m_size;
    size_t m_nozero;
    double *m_workspace;
    size_t m_work_len;
    double m_all_time;
    double m_CPU_time;

    matrix_args *m_args;
    pthread_t m_tid; // ID process

    fill_in_matrix *m_fill_matrix_handler;

    int m_error;

public:
    solver (const int argc, const char *argv[]);
    int parser (const int argc, const char *argv[]);
    int start_solver ();
    void *multithread_msr_solver (void* args);

    bool is_error ();

    int set_memmory ();

    int set_data ();

    double *get_solve ();

private:
    int minimal_residual_solver (const double *msr_matrix, const int *indexes, const int n,
                                 const double *b, double *x, double *workspace,
                                 const int my_rank, const int total_thread, const int maxit, const double eps);

    void *msr_solver(void* args);

    double compute_residual (const double *msr_matrix, const int *indexes, const int n,
                             const double *rhs, const double *x, double *vect, const int my_rank, const int total_thread);
};

#endif // SOLVER_H
