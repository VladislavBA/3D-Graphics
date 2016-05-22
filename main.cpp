
#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include <QMenuBar>
#include <QMenu>

#include "scene.h"

#define A -1
#define B 1
#define C -1
#define D 1
#define M 10
#define N 10

/*
static void create_function_menu (QMenu *function_menu, QMainWindow *graph_area)
  {
    function_menu->addAction ("&Const &f(x) = 1", graph_area, SLOT (change_const ()), Qt::Key_1);
    function_menu->addAction ("&Linear &f(x) = x", graph_area, SLOT (change_linear ()), Qt::Key_2);
    function_menu->addAction ("&Quadratic &f(x) = x^2", graph_area, SLOT (change_quadratic ()), Qt::Key_3);
    function_menu->addAction ("&Coub &f(x) = x^3", graph_area, SLOT (change_coub ()), Qt::Key_4);
    function_menu->addAction ("&Quaddro &f(x) = x^4", graph_area, SLOT (change_quaddro ()), Qt::Key_5);
    function_menu->addAction ("&Exponent &f(x) = e^x", graph_area, SLOT (change_exp ()), Qt::Key_6);
    function_menu->addAction ("Input&Function &f(x)", graph_area, SLOT (change_input ()), Qt::Key_7);
  }
static void create_show_menu (QMenu *show_menu, QMainWindow *graph_area)
  {
    show_menu->addAction ("&Newton", graph_area, SLOT (show_newton ()), Qt::Key_Q);
    show_menu->addAction ("&Spline", graph_area, SLOT (show_spline ()), Qt::Key_W);
    show_menu->addAction ("&Newton and &Spline", graph_area, SLOT (show_newton_spline ()), Qt::Key_E);
    show_menu->addAction ("&CoubSplineResidual", graph_area, SLOT (show_errors ()), Qt::Key_R);
  }
static void create_point_menu (QMenu *point_menu, QMainWindow *graph_area)
  {
    point_menu->addAction ("Increase&pointsNumber", graph_area, SLOT (increase_points ()), Qt::Key_Up);
    point_menu->addAction ("Decrease&pointsNumber", graph_area, SLOT (decrease_points ()), Qt::Key_Down);
  }
*/
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  QMainWindow *main_window = new QMainWindow ();  // Window application
/*
  QMenuBar *tool_bar = new QMenuBar (main_window); // ToolBar with menu application
  QMenu *function_menu = new QMenu ("&Function");
  QMenu *show_menu = new QMenu ("&Show");
  QMenu *point_menu = new QMenu ("&Edit_Point");
*/
  Scene3D *OpenGL_scene = new  Scene3D (main_window);
/*
  create_function_menu (function_menu, main_window);
  create_show_menu (show_menu, main_window);
  create_point_menu (point_menu, main_window);

  tool_bar->addMenu (function_menu);
  tool_bar->addMenu (show_menu);
  tool_bar->addMenu (point_menu);
*/
  main_window->setWindowTitle ("3D-Graphics");
  main_window->resize (1000, 1000);
  OpenGL_scene->resize (1000, 1000);
  main_window->show ();
  OpenGL_scene->show ();

  int app_error = app.exec();

  delete main_window;
  return app_error;
}
