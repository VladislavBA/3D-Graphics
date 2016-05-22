#ifndef MESH_H
#define MESH_H

#include <QPainter>

class mesh
{

private:
  QPointF initial_point_;
  // i.p._________
  //  |           |
  //  |           |
  //  |___________|
  //
  double width_, hight_; //< the rectangular size
  double cut_width_, cut_hight_; //< the cut rectangular coordinates
  int width_nodes_count_, hight_nodes_count_; //< the sizes of mesh
  int boundary_row_, boundary_colomn_; //< the last row and colomn in mesh
  QPointF *nodes_; // massive of nodes of mesh
  double *values_; // massive of valus in nodes
  int nodes_count_;

public:
  mesh ();
  mesh (double width, double hight, double cut_hight, double cut_width,
        int width_nodes_count, int hight_nodes_count, QPointF init_point);
  ~mesh ();
  int configurate_nodes ();
  QPointF *get_nodes ();
  double get_width () const;
  double get_hight () const;
  int get_nodes_count () const;
  int get_boundary_row () const;
  int get_boundary_colomn () const;
  int configurate_mesh (int width_point, int hight_point);
  void set_nodes_values ();
  int get_mesh_number (const int pos_x, const int pos_y);
  int get_coord_mesh (const int global_pos, int &pos_x, int &pos_y);
};

#endif // MESH_H
