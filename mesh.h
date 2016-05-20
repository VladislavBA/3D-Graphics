#ifndef MESH_H
#define MESH_H

#include <QPainter>

class mesh
{

private:
  double initial_point_;
  //  _____________
  //  |           |
  //  |           |
  //  |___________|
  // i.p.
  double width_, hight_; //< the rectangular size
  double cut_width_, cut_hight_; //< the cut rectangular coordinates
  int width_nodes_count_, hight_nodes_count_; //< the sizes of mesh
  int boundary_row_, boundary_colomn_;
  QPointF *nodes_; // massive of nodes of mesh
  int nodes_count_;

public:
  mesh ();
  mesh (double width, double hight, double cut_hight, double cut_width,
        int width_nodes_count, int hight_nodes_count, double init_point = 0);
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
};

#endif // MESH_H
