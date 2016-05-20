#ifndef MATRIX_ARGS_H
#define MATRIX_ARGS_H

class matrix_args
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

public:
  matrix_args ();
  ~matrix_args ();

  void matrix_args_init (double *matrix, int *ind, double *x, double *b, int n,
                         int nz, int my_rank, int total_thread,
                         double* workspace, int work_len, int width);

  int get_my_rank () const;
  int get_total_thread () const;
  double *get_matrix ();
  double *get_x ();
  double *get_rhs ();
  int get_size () const;
  int *get_indexes ();
  int get_width () const;
  double *get_workspace ();
  int get_nozero () const;
  double get_thread_time ();
  void set_thread_time (const double current_time);

};
#endif // MATRIX_ARGS_H
