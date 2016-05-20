
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
static void create_function_menu (QMenu *function_menu, Scene3D *graph_area)
  {
    function_menu->addAction ("&Const &f(x) = 1", graph_area, SLOT (change_const ()), Qt::Key_1);
    function_menu->addAction ("&Linear &f(x) = x", graph_area, SLOT (change_linear ()), Qt::Key_2);
    function_menu->addAction ("&Quadratic &f(x) = x^2", graph_area, SLOT (change_quadratic ()), Qt::Key_3);
    function_menu->addAction ("&Coub &f(x) = x^3", graph_area, SLOT (change_coub ()), Qt::Key_4);
    function_menu->addAction ("&Quaddro &f(x) = x^4", graph_area, SLOT (change_quaddro ()), Qt::Key_5);
    function_menu->addAction ("&Exponent &f(x) = e^x", graph_area, SLOT (change_exp ()), Qt::Key_6);
    function_menu->addAction ("Input&Function &f(x)", graph_area, SLOT (change_input ()), Qt::Key_7);
  }

static void create_show_menu (QMenu *show_menu, Scene3D *graph_area)
  {
    show_menu->addAction ("&Newton", graph_area, SLOT (show_newton ()), Qt::Key_Q);
    show_menu->addAction ("&Spline", graph_area, SLOT (show_spline ()), Qt::Key_W);
    show_menu->addAction ("&Newton and &Spline", graph_area, SLOT (show_newton_spline ()), Qt::Key_E);
    show_menu->addAction ("&CoubSplineResidual", graph_area, SLOT (show_errors ()), Qt::Key_R);
  }

static void create_zoom_menu (QMenu *zoom_menu, Scene3D *graph_area)
  {
    zoom_menu->addAction ("Zoom&In", graph_area, SLOT (zoom_in ()), Qt::Key_Plus);
    zoom_menu->addAction ("Zoom&Out", graph_area, SLOT (zoom_out ()), Qt::Key_Minus);
    zoom_menu->addAction ("Move&Left", graph_area, SLOT (zoom_left ()), Qt::Key_Left);
    zoom_menu->addAction ("Move&Right", graph_area, SLOT (zoom_right ()), Qt::Key_Right);

  }
static void create_point_menu (QMenu *point_menu, Scene3D *graph_area)
  {
    point_menu->addAction ("Increase&pointsNumber", graph_area, SLOT (increase_points ()), Qt::Key_Up);
    point_menu->addAction ("Decrease&pointsNumber", graph_area, SLOT (decrease_points ()), Qt::Key_Down);
  }
*/
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
//  QMainWindow *main_window = new QMainWindow ();  // Window application
/*
  QMenuBar *tool_bar = new QMenuBar (main_window); // ToolBar with menu application
  QMenu *function_menu = new QMenu ("&Function");
  QMenu *show_menu = new QMenu ("&Show");
  QMenu *zoom_menu = new QMenu ("&Zoom");
  QMenu *point_menu = new QMenu ("&Edit_Point");
*/
  Scene3D *OpenGL_scene = new  Scene3D ();
/*
  create_function_menu (function_menu, OpenGL_scene);
  create_show_menu (show_menu, OpenGL_scene);
  create_zoom_menu (zoom_menu, OpenGL_scene);
  create_point_menu (point_menu, OpenGL_scene);

  tool_bar->addMenu (show_menu);
  tool_bar->addMenu (function_menu);
  tool_bar->addMenu (zoom_menu);
  tool_bar->addMenu (point_menu);
*/

  OpenGL_scene->setWindowTitle ("3D-Graphics");
  OpenGL_scene->resize (1000, 1000);
  OpenGL_scene->show ();

  int app_error = app.exec();

  return app_error;
}
