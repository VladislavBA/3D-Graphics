#ifndef SCENE3D_H
#define SCENE3D_H

#include <QGLWidget>

class mesh;

class Scene3D : public QGLWidget
{
private:
  GLfloat x_rotate_;
  GLfloat y_rotate_;
  GLfloat z_rotate_;
  GLfloat z_translate_;
  GLfloat zoom_scalar;

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
};
#endif
