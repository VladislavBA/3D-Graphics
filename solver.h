#ifndef SOLVER_H
#define SOLVER_H

#include "multithread.h"

class matrix_args;

class solver
{
private:
    double *msr_matrix_;
    int *indexes_;
    double *x_;
    double *b_;
    int n_;
    int nz_;
    int my_rank_;
    int total_thread_;
    double *workspace_;
    int work_len_;
    int width_;
    double thread_time_;

    matrix_args *args_;
    int total_thread = 0;
    pthread_t tid = 0; // ID process

    matrix_args *args = nullptr;

public:
    solver ();

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
