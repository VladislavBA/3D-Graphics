#ifndef ITER_ALG_H
#define ITER_ALG_H

int minimal_residual_solver (const double *msr_matrix, const int *indexes, const int n,
                             const double *b, double *x, double *workspace,
                             const int my_rank, const int total_thread, const int maxit, const double eps);

void *msr_solver(void* args);

double compute_residual (const double *msr_matrix, const int *indexes, const int n,
                         const double *rhs, const double *x, double *vect, const int my_rank, const int total_thread);

#endif // ITER_ALG_H
