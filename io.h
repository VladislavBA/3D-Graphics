#ifndef IO_H
#define IO_H

int create_msr_matrix (double *msr_matrix, int *indexes, const int n,
                       const int my_rank, const int process_number);

int read_msr_matrix_from_file (double *msr_matrix, int *indexes, const int n,
                               const char *file,
                               const int my_rank, const int process_number);
int is_symmetric_msr_matrix (const int *indexes, const int n, const int my_rank);

int create_msr_band_matrix (double *msr_matrix, int *indexes, const int width, const int n,
                            const int my_rank, const int process_number);

void print_msr_NF (const double *msr_matrix, const int *indexes, const int nz, const int my_rank);

void print_msr_matrix_SF (const double *msr_matrix, const int *indexes, const int n, const int my_rank);

void set_rhs_vector (const double *msr_matrix, const int *indexes, double *rhs, const int n, const int my_rank, const int total_thread);

void set_init_solve_vector (double *x, const int n, const int my_rank, const int total_thread);

void print_vector (const double *vector, const int len);

#endif // IO_H
