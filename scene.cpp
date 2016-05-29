#include <QtGui>
#include <math.h>
#include "stdio.h"

#include "mesh.h"

#include "scene.h"

const static float pi=3.141593, k=pi/180;

// 2 * 3
GLfloat VertexArray[15][3];
GLfloat ColorArray[15][3];
GLubyte IndexArray[16][3];

Scene3D::Scene3D (QWidget* parent) : QGLWidget (parent)
{
   x_rotate_ = -95; y_rotate_ = 5; z_rotate_ = -120;
   z_translate_ = 0; zoom_scalar = 1;
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
    (void)x;
    (void)y;
  return 0.2;
}

void Scene3D::getVertexArray ()
{
  double a = -1;
  double b = 1;
  double c = -1;
  double d = 1;
  int n = 3;
  int m = 2;
  double eps_ab = (b - a) / (2 * m - 2);
  double eps_cd = (d - c) / (2 * n - 2);
  int len_m = 2 * m - 1;
  int len_n = 2 * n - 1;

  for (int i = 0; i < len_m; i++)
    {
      for (int j = 0; j < len_n; j++)
        {
          VertexArray[i * len_n + j][0] = 1 - i * eps_ab;
          VertexArray[i * len_n + j][1] = j * eps_cd;
          VertexArray[i * len_n + j][2] = function (i * eps_ab, j * eps_cd);
//          printf("Vert[%d] = %f %f %f\n", VertexArray[i * ])
        }
    }

}

/*
void Scene3D::getVertexArray ()
{
   GLfloat R=0.75;

   GLfloat a=4*R/sqrt(10+2*sqrt(5));
   GLfloat alpha=acos((1-a*a/2/R/R));

   VertexArray[0][0]=0;
   VertexArray[0][1]=0;
   VertexArray[0][2]=R;

   VertexArray[1][0]=R*sin(alpha)*sin(0);
   VertexArray[1][1]=R*sin(alpha)*cos(0);
   VertexArray[1][2]=R*cos(alpha);

   VertexArray[2][0]=R*sin(alpha)*sin(72*k);
   VertexArray[2][1]=R*sin(alpha)*cos(72*k);
   VertexArray[2][2]=R*cos(alpha);

   VertexArray[3][0]=R*sin(alpha)*sin(2*72*k);
   VertexArray[3][1]=R*sin(alpha)*cos(2*72*k);
   VertexArray[3][2]=R*cos(alpha);

   VertexArray[4][0]=R*sin(alpha)*sin(3*72*k);
   VertexArray[4][1]=R*sin(alpha)*cos(3*72*k);
   VertexArray[4][2]=R*cos(alpha);

   VertexArray[5][0]=R*sin(alpha)*sin(4*72*k);
   VertexArray[5][1]=R*sin(alpha)*cos(4*72*k);
   VertexArray[5][2]=R*cos(alpha);

   VertexArray[6][0]=R*sin(pi-alpha)*sin(-36*k);
   VertexArray[6][1]=R*sin(pi-alpha)*cos(-36*k);
   VertexArray[6][2]=R*cos(pi-alpha);

   VertexArray[7][0]=R*sin(pi-alpha)*sin(36*k);
   VertexArray[7][1]=R*sin(pi-alpha)*cos(36*k);
   VertexArray[7][2]=R*cos(pi-alpha);

   VertexArray[8][0]=R*sin(pi-alpha)*sin((36+72)*k);
   VertexArray[8][1]=R*sin(pi-alpha)*cos((36+72)*k);
   VertexArray[8][2]=R*cos(pi-alpha);

   VertexArray[9][0]=R*sin(pi-alpha)*sin((36+2*72)*k);
   VertexArray[9][1]=R*sin(pi-alpha)*cos((36+2*72)*k);
   VertexArray[9][2]=R*cos(pi-alpha);

   VertexArray[10][0]=R*sin(pi-alpha)*sin((36+3*72)*k);
   VertexArray[10][1]=R*sin(pi-alpha)*cos((36+3*72)*k);
   VertexArray[10][2]=R*cos(pi-alpha);

   VertexArray[11][0]=0;
   VertexArray[11][1]=0;
   VertexArray[11][2]=-R;
}
*/
void Scene3D::getColorArray ()
{
   for (int i=0; i<15; i++)
   {
      ColorArray[i][0]=0.1f*(qrand()%15);
      ColorArray[i][1]=0.1f*(qrand()%15);
      ColorArray[i][2]=0.1f*(qrand()%15);
   }
}

void Scene3D::getIndexArray()
{
   int m = 2;
   int n = 3;
   int rect_on_m = 2 * m - 2;
   int rect_on_n = 2 * n - 2;

   for (int i = 0; i < rect_on_m; i++)
   {
     for (int j = 0; j < rect_on_n; j++)
     {

            IndexArray[i * rect_on_n + j][0] = i * rect_on_n + j;
            IndexArray[i * rect_on_n + j][1] = i * rect_on_n + (j + 1);
            IndexArray[i * rect_on_n + j][2] = (i + 1) * rect_on_n + j;
            j++;
            IndexArray[i * rect_on_n + j][0] = i * rect_on_n + j;
            IndexArray[i * rect_on_n + j][1] = (i + 1) * rect_on_n + j;
            IndexArray[i * rect_on_n + j][2] = (i + 1) * rect_on_n + (j - 1);
            j--;
     }
   }
/*
   IndexArray[0][0]=0;
   IndexArray[0][1]=6;
   IndexArray[0][2]=1;

   IndexArray[1][0]=1;
   IndexArray[1][1]=6;
   IndexArray[1][2]=7;

   IndexArray[2][0]=1;
   IndexArray[2][1]=7;
   IndexArray[2][2]=2;

   IndexArray[3][0]=2;
   IndexArray[3][1]=7;
   IndexArray[3][2]=8;

   IndexArray[4][0]=2;
   IndexArray[4][1]=8;
   IndexArray[4][2]=3;

   IndexArray[5][0]=3;
   IndexArray[5][1]=8;
   IndexArray[5][2]=9;

   IndexArray[6][0]=3;
   IndexArray[6][1]=1;
   IndexArray[6][2]=2;

   IndexArray[7][0]=7;
   IndexArray[7][1]=2;
   IndexArray[7][2]=8;

   IndexArray[8][0]=8;
   IndexArray[8][1]=2;
   IndexArray[8][2]=3;

   IndexArray[9][0]=8;
   IndexArray[9][1]=3;
   IndexArray[9][2]=9;

   IndexArray[10][0]=9;
   IndexArray[10][1]=3;
   IndexArray[10][2]=4;

   IndexArray[11][0]=9;
   IndexArray[11][1]=4;
   IndexArray[11][2]=10;

   IndexArray[12][0]=10;
   IndexArray[12][1]=4;
   IndexArray[12][2]=5;

   IndexArray[13][0]=10;
   IndexArray[13][1]=5;
   IndexArray[13][2]=6;

   IndexArray[14][0]=6;
   IndexArray[14][1]=5;
   IndexArray[14][2]=1;

   IndexArray[15][0]=7;
   IndexArray[15][1]=11;
   IndexArray[15][2]=6;

   IndexArray[16][0]=8;
   IndexArray[16][1]=11;
   IndexArray[16][2]=7;
   */

}

void Scene3D::drawFigure ()
{
   glVertexPointer(3, GL_FLOAT, 0, VertexArray);
   glColorPointer(3, GL_FLOAT , 0, ColorArray);
//   qglColor(Qt::green);
   glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, IndexArray);
}
