#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include <QMenuBar>
#include <QMenu>

#include "mesh.h"
#include "scene.h"
#include "solver.h"

#define DEFAULT_WIDTH 1.
#define DEFAULT_HIGHT 1.
#define DEFAULT_CUT_WIDHT 0
#define DEFAULT_CUT_HIGHT 0
#define DEFAULT_INIT_POINT 0,0
#define DEFAULT_M 10
#define DEFAULT_N 10


int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QMainWindow *main_window = new QMainWindow ();  // Window application

  Scene3D *OpenGL_scene = new Scene3D (main_window);

  int error_parser = OpenGL_scene->parser (argc, argv);
  if (error_parser < 0)
    return -1;

  solver *solver_for_model = new solver ();

  main_window->setWindowTitle ("3D-Graphics");
  main_window->resize (1000, 1000);
  OpenGL_scene->resize (1000, 1000);
  main_window->show ();
  OpenGL_scene->show ();

  int app_error = app.exec();

  delete solver_for_model;
  delete main_window;

  return app_error;
}
