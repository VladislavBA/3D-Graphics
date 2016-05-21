#include "mesh.h"


mesh::mesh ()
{
  width_ = hight_ = cut_hight_ = cut_width_ = 0;
  width_nodes_count_ = hight_nodes_count_ = nodes_count_ = 0;
  boundary_colomn_ = boundary_row_ = 0;
  nodes_ = 0;
}

mesh::mesh (double width, double hight, double cut_hight, double cut_width, int width_nodes_count, int hight_nodes_count, QPointF init_point)
{
  width_ = width;
  hight_ = hight;
  cut_hight_ = cut_hight;
  cut_width_ = cut_width;
  width_nodes_count_ = width_nodes_count;
  hight_nodes_count_ = hight_nodes_count;
  nodes_count_ = configurate_nodes ();
  nodes_ = new QPointF[nodes_count_];
  initial_point_ = init_point;

}

mesh::~mesh ()
{
  if (nodes_)
    {
      delete [] nodes_;
    }
}

int mesh::configurate_nodes ()
{
  if (width_nodes_count_ <= 0 || hight_nodes_count_ <= 0)
    return 0;                                             //< we haven't nodes

  double dist_width = width_ / width_nodes_count_;
  double dist_hight = hight_ / hight_nodes_count_;

  boundary_row_ = (hight_ - cut_hight_) / dist_hight;
  boundary_colomn_ = (width_ - cut_width_) / dist_width;

  const double boundary_eps = 1e-8;                         //< take addition nodes

      if (width_ - boundary_colomn_ * dist_width > boundary_eps)
          boundary_row_++;
      if (hight_ - boundary_row_ * dist_hight > boundary_eps)
          boundary_colomn_++;

      return boundary_row_ * width_nodes_count_ + (hight_nodes_count_ - boundary_row_) * boundary_colomn_;
}

QPointF *mesh::get_nodes ()
{
  return nodes_;
}

double mesh::get_width () const
{
  return width_;
}

double mesh::get_hight () const
{
  return hight_;
}

int mesh::get_nodes_count () const
{
  return nodes_count_;
}

int mesh::get_boundary_row () const
{
  return boundary_row_;
}

int mesh::get_boundary_colomn () const
{
  return boundary_colomn_;
}

int mesh::configurate_mesh (int width_point, int hight_point)
{
  bool is_resize_mem = false;

  if ((width_point != width_nodes_count_ || hight_point != hight_nodes_count_) && nodes_ != 0)
    {
      is_resize_mem = true;
      delete [] nodes_;
    }
  if (is_resize_mem == false)
    return -1;
  width_nodes_count_ = width_point;
  hight_nodes_count_ = hight_point;

  nodes_count_ = configurate_nodes ();

  nodes_ = new QPointF [nodes_count_];

  return 0;
}

void mesh::set_nodes_values ()
{
  const double step_value_x = width_ / width_nodes_count_;
  const double step_value_y = hight_ / hight_nodes_count_;

  // set rectangular upper part
  for (int i = 0; i <= boundary_row_; i++)
    {
      for (int j = 0; j < boundary_colomn_; j++)
        {
          nodes_[i * boundary_colomn_ + j].setX (initial_point_.x () + j * step_value_x);
          nodes_[i * boundary_colomn_ + j].setY (initial_point_.y () - i * step_value_y);
        }
    }
  for (int i = boundary_row_ + 1; i < hight_nodes_count_; i++)
    {
      for (int j = 0; j < width_nodes_count_; j++)
        {
          nodes_[i * width_nodes_count_ + j].setX (initial_point_.x () + j * step_value_x);
          nodes_[i * width_nodes_count_ + j].setY (initial_point_.y () - i * step_value_y);
        }
    }
}

