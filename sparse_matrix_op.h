#ifndef SPARSE_MATRIX_OP_H
#define SPARSE_MATRIX_OP_H

void multi_jacobi_preconditioner (const double *msr_matrix, const int n,          //< MSR-stored matrix, size n
                                  double *y, const double *f,                     //< vector and rhs
                                  const int my_rank, const int total_thread);   //< multithread

void msr_multiply_matrix (const double *msr_matrix, const int* indexes, const int n, //< MSR-stored matrix, size n
                          const double *vector, double *result,                      //< vector and prec_vector
                          const int my_rank, const int total_thread);              //< multithread

void msr_preconditional (const double *msr_matrix, const int *indexes, const int n,  //< MSR-stored matrix, size n
                         const double *u, double *b,                                 //< vector and prec_vector
                         const int my_rank, const int total_thread);                   //< multithread

double inner_product (const double *x, const double *y, const int n,                //< scalar product two vectors
                      const int my_rank, const int process_number);                  //< multithread

void linear_combination (double *y, const double *z, const double scalar,
                         const int n, const int my_rank, const int total_thread);

void msr_residual (const double *msr_matrix, const int *indexes, const int n,
                   const double *x, const double *b, double *r,
                   const int my_rank, const int total_thread);

double compute_vector_inf_norm (const double *vector, const int n, const int my_rank, const int total_thread);

void copy (const double *src, double *dst, const int n, const int my_rank, const int total_thread);

#endif // SPARSE_MATRIX_OP_H
