#ifndef FILL_IN_MATRIX_H
#define FILL_IN_MATRIX_H



class fill_in_matrix
{

private:
    int cut_row_;
    int cut_col_;
    double J_;

    void pattern_1 (int* index, const int i, const int j, const int m, const int n);
    void pattern_2 (int* index, const int i, const int j, const int m, const int n);
    void pattern_3 (int* index, const int i, const int j, const int m, const int n);
    void pattern_4 (int* index, const int i, const int j, const int m, const int n);

public:
    fill_in_matrix (const int cut_row, const int cut_col, const double jacobian);
    ~fill_in_matrix ();

    int get_nz_elements_current_node (const int i, const int j, const int row_cut, const int col_cut, const int m, const int n, int *index = 0);
    int get_nozero_elements (const int m, const int n, const int my_rank, const int total_thread);
    int fill_indexes (const int m, const int n, int *indexes);
    void fillin_msr_matrix (double *msr_matrix, const int *indexes, const int m, const int n, const int nz, const int my_rank, const int total_thread);
    void set_msr_matrix ();
    void fillin_rhs (double *rhs, int n, const int my_rank, const int total_thread);
    double rhs_element (const int global_num);
};

#endif // FILL_IN_MATRIX_H
