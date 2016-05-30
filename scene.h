#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>

class mesh;
class fill_in_matrix;

struct input_data
{
  // rectangular
  GLdouble left_down_point_a;
  GLdouble right_down_point_b;
  GLdouble left_down_point_c;
  GLdouble left_up_point_d;

  // cut rectangular
  GLdouble left_down_cut_p;
  GLdouble right_up_cut_q;

  size_t num_point_mesh_m;
  size_t num_point_mesh_n;

  int total_thread;
};

struct matrix_data
{
  double *s_msr_matrix;
  double *s_workspace;
  double *s_solve;
  double *s_rhs;
  int s_size_n;
  int s_size_m;
  int *s_indexes_matrix;
  int s_nozero;
  int s_total_thread;
  int s_workspace_len;
};

struct scene_data
{
  GLfloat x_rotate_;
  GLfloat y_rotate_;
  GLfloat z_rotate_;
  GLfloat z_translate_;
  GLfloat zoom_scalar;
};

class Scene3D : public QGLWidget
{
private:

  scene_data m_settings;
  matrix_data m_matrix;
  input_data m_in_data;

  fill_in_matrix *matrix_filler;

  int m_;
  int n_;

  GLdouble *nodes_mesh;
  GLuint *index_nodes_mesh;
  GLdouble *colors;

  int allnodes;
  int allareas;
  int alltriangles;

  QPoint mouse_ptr_pos;

  mesh *area_mesh_;

  void scale_plus();
  void scale_minus();
  void rotate_up();
  void rotate_down();
  void rotate_left();
  void rotate_right();
  void translate_down();
  void translate_up();
  void defaultScene();

  void drawAxis();

  void getVertexArray();
  void getColorArray();
  void getIndexArray();
  void drawFigure();



protected:
  void initializeGL();
  void resizeGL(int nWidth, int nHeight);
  void paintGL();
  void mousePressEvent(QMouseEvent* pe);
  void mouseMoveEvent(QMouseEvent* pe);
  void mouseReleaseEvent(QMouseEvent* pe);
  void wheelEvent(QWheelEvent* pe);
  void keyPressEvent(QKeyEvent* pe);

public:
  Scene3D(QWidget* parent = 0);
  int set_mesh (mesh *init_painted_mesh);
  int parser (int argc, char *argv);
  int fill_matrix();

};
#endif
