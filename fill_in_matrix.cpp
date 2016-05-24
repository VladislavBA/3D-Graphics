#include "fill_in_matrix.h"

#include "multithread.h"

void fill_in_matrix::pattern_1 (int *index, const int i, const int j)
{
    index[0] = get_mesh_number (i - 1, j);
    index[1] = get_mesh_number (i - 1, j + 1);
    index[2] = get_mesh_number (i, j - 1);
    index[3] = get_mesh_number (i, j + 1);
    index[4] = get_mesh_number (i + 1, j - 1);
    index[5] = get_mesh_number (i + 1, j);
}

void fill_in_matrix::pattern_2 (int *index, const int i, const int j)
{
    if (i == 0)
    {
        index[0] = get_mesh_number (i, j + 1);
        index[1] = get_mesh_number (i + 1, j);
    }
    else
    {
        index[0] = get_mesh_number (i - 1, j);
        index[1] = get_mesh_number (i, j - 1);
    }
}

void fill_in_matrix::pattern_3 (int *index, const int i, const int j)
{
    if (i == 0)
    {
        index[0] = get_mesh_number (i, j - 1);
        index[1] = get_mesh_number (i + 1, j - 1);
        index[2] = get_mesh_number (i + 1, j);
    }
    else
    {
        index[0] = get_mesh_number (i - 1, j);
        index[1] = get_mesh_number (i - 1, j + 1);
        index[2] = get_mesh_number (i, j + 1);
    }
}

void fill_in_matrix::pattern_4 (int *index, const int i, const int j)
{
    if (i == 0)
    {
        index[0] = get_mesh_number (i, j - 1);
        index[1] = get_mesh_number (i, j + 1);
        index[2] = get_mesh_number (i + 1, j - 1);
        index[3] = get_mesh_number (i + 1, j);
    }
    if (j == 0)
    {
        index[0] = get_mesh_number (i - 1, j);
        index[1] = get_mesh_number (i - 1, j + 1);
        index[2] = get_mesh_number (i, j + 1);
        index[3] = get_mesh_number (i + 1, j);
    }
    if (i == m_ || i == cut_row_)
    {
        index[0] = get_mesh_number (i, j - 1);
        index[1] = get_mesh_number (i - 1, j);
        index[2] = get_mesh_number (i - 1, j + 1);
        index[3] = get_mesh_number (i, j + 1);
    }
    if (j == n_ || j == cut_col_)
    {
        index[0] = get_mesh_number (i - 1, j);
        index[1] = get_mesh_number (i + 1, j);
        index[2] = get_mesh_number (i + 1, j - 1);
        index[3] = get_mesh_number (i, j - 1);
      }
}

double fill_in_matrix::rhs_pattern_count_6 (const int i, const int j)
{
  constexpr int triangular_num = 6;
  constexpr int start_triangular = 0;
  double result_sum = 0.;
  for (int k = start_triangular; i < triangular_num; i++)
    result_sum += compute_curr_triangular (k, i, j);

  return result_sum;
}

double fill_in_matrix::rhs_pattern_count_6_special(const int i, const int j)
{
  constexpr int triangular_num = 5;
  double result_sum = 0.;
  constexpr int start_triangular = 3;
  for (int k = start_triangular; i < start_triangular + triangular_num; i++)
    result_sum += compute_curr_triangular (k % 6, i, j);

  return result_sum;
}

double fill_in_matrix::rhs_pattern_count_4_up(const int i, const int j)
{
  constexpr int triangular_num = 3;
  double result_sum = 0.;
  constexpr int start_triangular = 2;
  for (int k = start_triangular; i < start_triangular + triangular_num; i++)
    result_sum += compute_curr_triangular (k, i, j);

  return result_sum;
}

double fill_in_matrix::rhs_pattern_count_4_left(const int i, const int j)
{
  constexpr int triangular_num = 3;
  double result_sum = 0.;
  constexpr int start_triangular = 0;
  for (int k = start_triangular; i < start_triangular + triangular_num; i++)
    result_sum += compute_curr_triangular (k, i, j);

  return result_sum;
}

double fill_in_matrix::rhs_pattern_count_4_down(const int i, const int j)
{
  constexpr int triangular_num = 3;
  double result_sum = 0.;
  constexpr int start_triangular = 0;
  for (int k = start_triangular; i < start_triangular + triangular_num; i++)
    result_sum += compute_curr_triangular (k, i, j);

  return result_sum;
}

fill_in_matrix::fill_in_matrix (const int cut_row, const int cut_col, const int m, const int n, const double jacobian)
{
  cut_row_ = cut_row;
  cut_col_ = cut_col;
  J_ = jacobian;
  m_ = m;
  n_ = n;
  N_ = m_ * n_;
}

fill_in_matrix::~fill_in_matrix () {}

int fill_in_matrix::get_nz_elements_current_node (const int i, const int j, int* index)
{
  nodes_type type_current_node_mesh = get_node_type (i, j);

  switch (type_current_node_mesh)
    {
    case FULL_NODE:
    case SPECIAL_POINT:
      pattern_1 (index, i, j);
      return 6;
    case UP_WALL:
    case LEFT_WALL:
    case DOWN_WALL:
    case RIGHT_WALL:
      pattern_4 (index, i, j);
      return 4;
    case LEFT_UP_CORNER:
    case RIGTH_DOWN_CORNER:
      pattern_2(index, i, j);
      return 2;
    case LEFT_DOWN_CORNER:
    case RIGHT_UP_CORNER:
      pattern_3 (index, i, j);
      return 3;
    case UNDEFINED:
      return 0;
    }
  return 0;
}

int fill_in_matrix::get_nozero_elements (const int my_rank, const int total_thread)
{
    int i1 = my_rank * m_;
    i1 /= total_thread;
    int i2 = (my_rank + 1) * m_;
    i2 = i2 / total_thread - 1;

    int nz = 0;

    for (int i = i1; i <= i2; i++)
    {
        for (int j = 0; j <= n_; j++)
        {
            nz += get_nz_elements_current_node (i, j);
        }
    }
    reduce_sum (&nz, 1, total_thread);
    return nz;
}

int fill_in_matrix::fill_indexes (int *indexes)
{
    constexpr int max_row_nz = 6;
    int ind_loc[max_row_nz];

    int global_index = 0;
    int NZ = N_;
    int nz = 0;

    for (int i = 0; i < m_; i++)
    {
        for (int j = 0; j < n_; j++, global_index++)
        {
          nz = get_nz_elements_current_node (i, j, ind_loc);
          indexes[global_index] = NZ;
          for (int s = 0; s < nz; s++)
          {
              indexes[NZ + s] = ind_loc[s];
          }
          NZ += nz;
        }
      indexes[global_index] = NZ;
    }
    return NZ;
}

void fill_in_matrix::fillin_msr_matrix (double *msr_matrix, const int *indexes, const int my_rank, const int total_thread)
{
  const int n_ = n_ * m_;
  int i1 = my_rank * n_;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * n_;
  i2 = i2 / total_thread - 1;
  const double I0 = (1. / 2) * J_;
  const double I1 = (1. / 12) * J_;
  int nz = 0;
  int begin = 0;

  for (int i = i1; i <= i2; i++)
  {
    msr_matrix[i] = I0;
    nz = indexes[i + 1] - indexes[i];
    begin = indexes[i];
    for (int j = 0; j < nz; j++)
    {
      msr_matrix[begin + j] = I1;
    }
    }
}

nodes_type fill_in_matrix::get_node_type(const int i, const int j)
{
  if ((i > 0 && j > 0 && i < m_ && j < n_ && ((i > cut_row_ && j < cut_col_) || i <= cut_row_)))
    return nodes_type::FULL_NODE;

  if (i == 0 && j > 0 && j < 0)
    return nodes_type::UP_WALL;

  if (j == (n_ - 1) && i > 0 && i < cut_row_)
    return nodes_type::RIGHT_WALL;

  if (i == cut_row_ && j > cut_col_ && j < n_ - 1)
    return nodes_type::DOWN_WALL;

  if (j == cut_col_&& i < (m_ - 1) && i > cut_row_)
    return nodes_type::RIGHT_WALL;

  if (i == m_ - 1 && j < cut_col_ && j > 0)
    return nodes_type::DOWN_WALL;

  if (j == 0 && i > 0 && i < m_ - 1)
    return nodes_type::LEFT_WALL;

  if (i == 0 && j == 0)
    return nodes_type::LEFT_UP_CORNER;

  if (i == 0 && j == n_ - 1)
    return nodes_type::RIGHT_UP_CORNER;

  if ((i == cut_row_ && j == n_ - 1) || (i == m_ - 1 && j == cut_col_))
    return nodes_type::RIGTH_DOWN_CORNER;

  if (i == m_ - 1 && j == 0)
    return nodes_type::LEFT_DOWN_CORNER;

  if (i == cut_row_ && j == cut_col_)
    return nodes_type:: SPECIAL_POINT;

  return nodes_type::UNDEFINED;
}

void fill_in_matrix::fillin_rhs (double *rhs, const int my_rank, const int total_thread)
{
  int i1 = my_rank * N_;
  i1 /= total_thread;
  int i2 = (my_rank + 1) * N_;
  i2 = i2 / total_thread - 1;

  for (int i = i1; i <= i2; i++)
    {
      rhs[i] = rhs_element (i);
    }

  synchronize (total_thread);
}

double fill_in_matrix::rhs_element (const int global_num)
{
  int i = 0;
  int j = 0;

  get_coord_mesh (global_num, i, j);

  nodes_type type_current_node = get_node_type (i, j);

  switch (type_current_node)
    {
    case FULL_NODE:
      return rhs_pattern_count_6 (i, j);
    case SPECIAL_POINT:
      return rhs_pattern_count_6_special (i, j);
    case UP_WALL:
      return rhs_pattern_count_4_up (i, j);
    case LEFT_WALL:
      return rhs_pattern_count_4_left (i, j);
    case DOWN_WALL:
      return rhs_pattern_count_4_down (i, j);
    case RIGHT_WALL:
      return rhs_pattern_count_4_right (i, j);
    case LEFT_UP_CORNER:
      return rhs_pattern_count_2_left (i, j);
    case RIGTH_DOWN_CORNER:
      return rhs_pattern_count_2_right (i, j);
    case LEFT_DOWN_CORNER:
      return rhs_pattern_count_3_left (i, j);
    case RIGHT_UP_CORNER:
      return rhs_pattern_count_3_right (i, j);
    case UNDEFINED:
      return 0;
    }
}

int fill_in_matrix::get_mesh_number (const int pos_x, const int pos_y)
{
    int global_mesh_position = 0;
    if (pos_y > cut_row_)
      {
        global_mesh_position = cut_col_ * n_;
        global_mesh_position += (pos_y - cut_row_) * cut_col_;
      }
    else
      {
        global_mesh_position = pos_y * n_;
      }

    global_mesh_position += pos_x;

    return global_mesh_position;
}

int fill_in_matrix::get_coord_mesh (const int global_pos, int &pos_x, int &pos_y)
{
    if (global_pos < 0)
        return -1;
    int row = global_pos / n_;

    if (row > cut_row_)
    {
        row = cut_col_ + (global_pos - cut_row_ * n_) / cut_col_;
        pos_y = row;
        pos_x = global_pos - (cut_row_ * n_ + (pos_y - cut_row_) * cut_col_);
    }
    else
    {
        pos_y = row;
        pos_x = global_pos - pos_y * n_;
    }
    return 0;
}
