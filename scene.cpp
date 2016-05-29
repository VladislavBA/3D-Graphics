#include <QtGui>
#include <math.h>
#include "stdio.h"

#include "mesh.h"

#include "scene.h"

#include "debug_function.h"

const static float pi=3.141593, k=pi/180;

// 2 * 3
GLfloat VertexArray[15][3];
GLfloat ColorArray[15][3];
GLubyte IndexArray[16][3];

Scene3D::Scene3D (QWidget* parent) : QGLWidget (parent)
{
   x_rotate_ = -95; y_rotate_ = 5; z_rotate_ = -120;
   z_translate_ = 0; zoom_scalar = 1;

   m_ = 2;
   n_ = 3;
   allnodes = (2 * n_ - 1) * (2 * m_ - 1);
   nodes_mesh = new GLdouble[3 * allnodes];
   allareas = (2 * n_ - 2) * (2 * m_ - 2);
   index_nodes_mesh = new GLuint[3 * allareas];

   alltriangles = (2 * m_ - 2) * (2 * n_ - 2) * 2 * 3;

   colors = new GLdouble[3 * allnodes];

}

int Scene3D::set_mesh (mesh *init_painted_mesh)
{
    if (!init_painted_mesh)
        return -1;
    area_mesh_ = init_painted_mesh;
    return 0;
}

void Scene3D::initializeGL ()
{
   qglClearColor (Qt::white); // background color
   glEnable (GL_DEPTH_TEST); //
   glShadeModel (GL_FLAT);
   glEnable (GL_FRONT_FACE); // set config for special paint

   getVertexArray ();
   getColorArray ();
   getIndexArray ();

   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_COLOR_ARRAY);
}

void Scene3D::resizeGL (int nWidth, int nHeight)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   GLfloat ratio = (GLfloat)nHeight / (GLfloat)nWidth;

   if (nWidth >= nHeight)
      glOrtho (-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -10.0, 1.0);
   else
      glOrtho (-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -10.0, 1.0);

   glViewport (0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::paintGL ()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();

   glScalef(zoom_scalar, zoom_scalar, zoom_scalar);
   glTranslatef(0.0f, z_translate_, 0.0f);
   glRotatef(x_rotate_, 1.0f, 0.0f, 0.0f);
   glRotatef(y_rotate_, 0.0f, 1.0f, 0.0f);
   glRotatef(z_rotate_, 0.0f, 0.0f, 1.0f);

   drawAxis ();
   drawFigure ();
}

void Scene3D::mousePressEvent (QMouseEvent* pe)
{
   mouse_ptr_pos = pe->pos ();
}

void Scene3D::mouseReleaseEvent (QMouseEvent* /*pe*/)
{

}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{
   x_rotate_ += 180 / zoom_scalar * (GLfloat)(pe->y ()-mouse_ptr_pos.y ()) / height ();
   z_rotate_ += 180 / zoom_scalar * (GLfloat)(pe->x ()-mouse_ptr_pos.x ()) / width ();

   mouse_ptr_pos = pe->pos ();

   updateGL ();
}

void Scene3D::wheelEvent (QWheelEvent* pe)
{
   if ((pe->delta ()) > 0)
     scale_plus ();
   else
     if ((pe->delta ()) < 0)
       scale_minus ();

   updateGL ();
}

void Scene3D::keyPressEvent (QKeyEvent* pe)
{
   switch (pe->key ())
   {
      case Qt::Key_Plus:
         scale_plus();
      break;

      case Qt::Key_Equal:
         scale_plus();
      break;

      case Qt::Key_Minus:
         scale_minus();
      break;

      case Qt::Key_Up:
         rotate_up();
      break;

      case Qt::Key_Down:
         rotate_down();
      break;

      case Qt::Key_Left:
        rotate_left();
      break;

      case Qt::Key_Right:
         rotate_right();
      break;

      case Qt::Key_Z:
         translate_down ();
      break;

      case Qt::Key_X:
         translate_up ();
      break;

      case Qt::Key_Space:
         defaultScene ();
      break;

      case Qt::Key_Escape:
         this->close ();
      break;
   }

   updateGL ();
}

void Scene3D::scale_plus ()
{
   zoom_scalar = zoom_scalar * 1.1;
}

void Scene3D::scale_minus ()
{
   zoom_scalar = zoom_scalar / 1.1;
}

void Scene3D::rotate_up ()
{
   x_rotate_ += 1.0;
}

void Scene3D::rotate_down ()
{
   x_rotate_ -= 1.0;
}

void Scene3D::rotate_left ()
{
   z_rotate_ += 1.0;
}

void Scene3D::rotate_right ()
{
   z_rotate_ -= 1.0;
}

void Scene3D::translate_down ()
{
   z_translate_ -= 0.05;
}

void Scene3D::translate_up ()
{
   z_translate_ += 0.05;
}

void Scene3D::defaultScene ()
{
   x_rotate_ = -95; y_rotate_ = 5; z_rotate_ = -120; z_translate_=0; zoom_scalar=1;
}

void Scene3D::drawAxis ()
{
   glLineWidth (3.0f);

   // Axis X
   glColor4f (1.00f, 0.00f, 0.00f, 1.0f); //< red
   glBegin (GL_LINES);
      glVertex3f ( 1.0f,  0.0f,  0.0f);
      glVertex3f (-1.0f,  0.0f,  0.0f);
   glEnd ();
   // Axis Y
   QColor halfGreen (0, 128, 0, 255); //< green
   qglColor (halfGreen);
   glBegin (GL_LINES);
      glVertex3f ( 0.0f,  1.0f,  0.0f);
      glVertex3f ( 0.0f, -1.0f,  0.0f);
    // Axis Z
      glColor4f (0.00f, 0.00f, 1.00f, 1.0f); //< blue
      glVertex3f ( 0.0f,  0.0f,  1.0f);
      glVertex3f ( 0.0f,  0.0f, -1.0f);
   glEnd();
}

static double function (const double x, const double y)
{
 //   (void)x;
   (void)y;
  return x;
}

void Scene3D::getVertexArray ()
{
  double a = -1;
  double b = 1;
  double c = -1;
  double d = 1;
  int n = 3;
  int m = 2;
  double eps_ab = (b - a) / (2 * n - 2);
  double eps_cd = (d - c) / (2 * m - 2);
  int len_m = 2 * m - 1;
  int len_n = 3 * (2 * n - 1);

  for (int i = 0; i < len_m; i++)
    {
      for (int j = 0; j < len_n; j+=3)
        {
          nodes_mesh[i * len_n + j] = -1 + j / 3 * eps_ab;
          nodes_mesh[i * len_n + j + 1] = 1 - i * eps_cd;
          nodes_mesh[i * len_n + j + 2] = function (-1 + j / 3 * eps_ab,1 - i * eps_cd);
        }
    }
}

void Scene3D::getColorArray ()
{
   for (int i=0; i<allnodes; i += 3)
   {
      colors[i]=0.1f*(qrand()%allnodes);
      colors[i + 1]=0.1f*(qrand()%allnodes);
      colors[i + 2]=0.1f*(qrand()%allnodes);
   }
}

void Scene3D::getIndexArray()
{
   int m = 2;
   int n = 3;
   const int nodes_on_my = 2 * m - 2;
   const int nodes_on_nx = 2 * n - 2;
   const int row_triangles = 3 * 2 * (2 * n - 2);

   for (int i = 0; i < nodes_on_my; i++)
   {
     for (int j = 0, s = 0; j < row_triangles; j += 6, s++)
     {
            index_nodes_mesh[i * row_triangles + j] = i * (nodes_on_nx + 1) + s;
            index_nodes_mesh[i * row_triangles + j + 1] = i * (nodes_on_nx + 1) + (s + 1);
            index_nodes_mesh[i * row_triangles + j + 2] = (i + 1) * (nodes_on_nx + 1) + s;

            index_nodes_mesh[i * row_triangles + j + 3] = i * (nodes_on_nx + 1) + s + 1;
            index_nodes_mesh[i * row_triangles + j + 4] = (i + 1) * (nodes_on_nx + 1) + s + 1;
            index_nodes_mesh[i * row_triangles + j + 5] = (i + 1) * (nodes_on_nx + 1) + s;
     }
   }
}

void Scene3D::drawFigure ()
{
   glVertexPointer(3, GL_DOUBLE, 0, nodes_mesh);
   glColorPointer(3, GL_DOUBLE , 0, colors);
//   qglColor(current_color);
   glDrawElements(GL_TRIANGLES, alltriangles, GL_UNSIGNED_INT, index_nodes_mesh);
//   glDrawElements(GL_TRIANGLE_STRIP, alltriangles, GL_UNSIGNED_INT, index_nodes_mesh);
}
