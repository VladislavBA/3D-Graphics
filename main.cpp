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

static int parser_error_handler (const int error, mesh *area_mesh)
{
    if (error >= 0)
        return 0;
    else
    {
        if (error == -1)
          {
            printf ("Set default settings...\n a = %f b = %f c = %f d = %f m = %d n = %d\nThreads  %d\n",
                    0., 1., 0., 1., 10, 10, 1);
            area_mesh->set_data_mesh(DEFAULT_WIDTH, DEFAULT_HIGHT, DEFAULT_CUT_WIDHT, DEFAULT_CUT_HIGHT, DEFAULT_M, DEFAULT_N, QPointF (DEFAULT_INIT_POINT));
            return 0;
          }
        return -1;
    }
    return 0;
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QMainWindow *main_window = new QMainWindow ();  // Window application

  mesh *area_mesh = new mesh ();

  int errors_cmd_parser = area_mesh->parse_arguments_cmd_line (argc, argv);
  int error = parser_error_handler (errors_cmd_parser, area_mesh);
  if (error < 0)
  {
      return -1;
  }

  Scene3D *OpenGL_scene = new Scene3D (main_window);

  int error_mesh = OpenGL_scene -> set_mesh(area_mesh);
  if (error_mesh < 0)
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
