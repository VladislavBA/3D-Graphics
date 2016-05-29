#ifndef FILL_IN_MATRIX_H
#define FILL_IN_MATRIX_H

class QPointF;

enum nodes_type
{
  UNDEFINED,
  FULL_NODE,
  LEFT_UP_CORNER,
  RIGTH_DOWN_CORNER,
  LEFT_DOWN_CORNER,
  RIGHT_UP_CORNER,
  UP_WALL,
  RIGHT_WALL,
  DOWN_WALL,
  LEFT_WALL,
  SPECIAL_POINT
};

class fill_in_matrix
{

private:
    int m_;
    int n_;
    int N_;
    int cut_row_;
    int cut_col_;
    double J_;
    double (*function_)(double, double);
    QPointF *nodes_;

    void pattern_1 (int* index, const int i, const int j);
    void pattern_2 (int* index, const int i, const int j);
    void pattern_3 (int* index, const int i, const int j);
    void pattern_4 (int* index, const int i, const int j);

    double rhs_pattern_count_6 (const int i, const int j);
    double rhs_pattern_count_6_special (const int i, const int j);
    double rhs_pattern_count_4_up (const int i, const int j);
    double rhs_pattern_count_4_left (const int i, const int j);
    double rhs_pattern_count_4_down (const int i, const int j);
    double rhs_pattern_count_4_right (const int i, const int j);
    double rhs_pattern_count_2_left (const int i, const int j);
    double rhs_pattern_count_2_right (const int i, const int j);
    double rhs_pattern_count_3_left (const int i, const int j);
    double rhs_pattern_count_3_right (const int i, const int j);

    double compute_curr_triangular (const int num_triangular, const int i, const int j);
    QPointF nodes (const int i, const int j);
    double function (const QPointF &point);

public:
    fill_in_matrix (const int cut_row, const int cut_col, const int m, const int n, const double jacobian);
    ~fill_in_matrix ();

    int get_nz_elements_current_node (const int i, const int j, int *index = 0);
    int get_nozero_elements (const int my_rank, const int total_thread);
    int fill_indexes (int *indexes);
    void fillin_msr_matrix (double *msr_matrix, const int *indexes, const int my_rank, const int total_thread);
    nodes_type get_node_type (const int i, const int j);
    void set_msr_matrix ();
    void fillin_rhs (double *rhs, const int my_rank, const int total_thread);
    double rhs_element (const int global_num);
    int get_mesh_number (const int pos_x, const int pos_y);
    int get_coord_mesh (const int global_pos, int &pos_x, int &pos_y);

};

#endif // FILL_IN_MATRIX_H
