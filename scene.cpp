#include <QtGui>
#include <math.h>
#include "scene.h"

const static float pi=3.141593, k=pi/180;

GLfloat VertexArray[12][3];
GLfloat ColorArray[12][3];
GLubyte IndexArray[20][3];

Scene3D::Scene3D (QWidget* parent) : QGLWidget(parent)
{
   x_rotate_=-95; y_rotate_=5; z_rotate_=-120; z_translate_=0; zoom_scalar=1;
}

void Scene3D::initializeGL ()
{
   qglClearColor(Qt::white);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glEnable(GL_CULL_FACE);

   getVertexArray();
   getColorArray();
   getIndexArray();

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
}

void Scene3D::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

   if (nWidth>=nHeight)
      glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
   else
      glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void Scene3D::paintGL()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glScalef(zoom_scalar, zoom_scalar, zoom_scalar);
   glTranslatef(0.0f, z_translate_, 0.0f);
   glRotatef(x_rotate_, 1.0f, 0.0f, 0.0f);
   glRotatef(y_rotate_, 0.0f, 1.0f, 0.0f);
   glRotatef(z_rotate_, 0.0f, 0.0f, 1.0f);

   drawAxis ();
   drawFigure ();
}

void Scene3D::mousePressEvent(QMouseEvent* pe)
{
   mouse_ptr_pos = pe->pos();
}

void Scene3D::mouseReleaseEvent(QMouseEvent* /*pe*/)
{

}

void Scene3D::mouseMoveEvent(QMouseEvent* pe)
{
   x_rotate_ += 180/zoom_scalar*(GLfloat)(pe->y()-mouse_ptr_pos.y())/height();
   z_rotate_ += 180/zoom_scalar*(GLfloat)(pe->x()-mouse_ptr_pos.x())/width();

   mouse_ptr_pos = pe->pos();

   updateGL();
}

void Scene3D::wheelEvent(QWheelEvent* pe)
{
   if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();

   updateGL();
}

void Scene3D::keyPressEvent(QKeyEvent* pe)
{
   switch (pe->key())
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
         translate_down();
      break;

      case Qt::Key_X:
         translate_up();
      break;

      case Qt::Key_Space:
         defaultScene();
      break;

      case Qt::Key_Escape:
         this->close();
      break;
   }

   updateGL();
}

void Scene3D::scale_plus()
{
   zoom_scalar = zoom_scalar*1.1;
}

void Scene3D::scale_minus()
{
   zoom_scalar = zoom_scalar/1.1;
}

void Scene3D::rotate_up()
{
   x_rotate_ += 1.0;
}

void Scene3D::rotate_down()
{
   x_rotate_ -= 1.0;
}

void Scene3D::rotate_left()
{
   z_rotate_ += 1.0;
}

void Scene3D::rotate_right()
{
   z_rotate_ -= 1.0;
}

void Scene3D::translate_down()
{
   z_translate_ -= 0.05;
}

void Scene3D::translate_up()
{
   z_translate_ += 0.05;
}

void Scene3D::defaultScene()
{
   x_rotate_=-90; y_rotate_=0; z_rotate_=0; z_translate_=0; zoom_scalar=1;
}

void Scene3D::drawAxis()
{
   glLineWidth(3.0f);

   // Axis X
   glColor4f(1.00f, 0.00f, 0.00f, 1.0f); //< red
   glBegin(GL_LINES);
      glVertex3f( 1.0f,  0.0f,  0.0f);
      glVertex3f(-1.0f,  0.0f,  0.0f);
   glEnd();
   // Axis Y
   QColor halfGreen(0, 128, 0, 255); //< green
   qglColor(halfGreen);
   glBegin(GL_LINES);
      glVertex3f( 0.0f,  1.0f,  0.0f);
      glVertex3f( 0.0f, -1.0f,  0.0f);
    // Axis Z
      glColor4f(0.00f, 0.00f, 1.00f, 1.0f); //< blue
      glVertex3f( 0.0f,  0.0f,  1.0f);
      glVertex3f( 0.0f,  0.0f, -1.0f);
   glEnd();
}

static double function(const double x, const double y)
{
  return x * x - y * y;
}

void Scene3D::getVertexArray ()
{
//  double a = -1;
//  double b = 1;
//  double c = -1;
//  double d = -1;
  int n = 3;
  int m = 4;
//  double eps_ab = (b - a) / m;
//  double eps_cd = (d - c) / n;

  for (int i = 0; i < m; i++)
    {
      for (int j = 0; j < n; j++)
        {
          VertexArray[i * n + j][0] = i;
          VertexArray[i * n + j][1] = j;
          VertexArray[i * n + j][2] = function (i, j);
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
void Scene3D::getColorArray()
{
   for (int i=0; i<12; i++)
   {
      ColorArray[i][0]=0.1f*(qrand()%11);
      ColorArray[i][1]=0.1f*(qrand()%11);
      ColorArray[i][2]=0.1f*(qrand()%11);
   }
}

void Scene3D::getIndexArray()
{
   IndexArray[0][0]=0;
   IndexArray[0][1]=2;
   IndexArray[0][2]=1;

   IndexArray[1][0]=0;
   IndexArray[1][1]=3;
   IndexArray[1][2]=2;

   IndexArray[2][0]=0;
   IndexArray[2][1]=4;
   IndexArray[2][2]=3;

   IndexArray[3][0]=0;
   IndexArray[3][1]=5;
   IndexArray[3][2]=4;

   IndexArray[4][0]=0;
   IndexArray[4][1]=1;
   IndexArray[4][2]=5;

   IndexArray[5][0]=6;
   IndexArray[5][1]=1;
   IndexArray[5][2]=7;

   IndexArray[6][0]=7;
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

   IndexArray[17][0]=9;
   IndexArray[17][1]=11;
   IndexArray[17][2]=8;

   IndexArray[18][0]=10;
   IndexArray[18][1]=11;
   IndexArray[18][2]=9;

   IndexArray[19][0]=6;
   IndexArray[19][1]=11;
   IndexArray[19][2]=10;
}

void Scene3D::drawFigure ()
{
   glVertexPointer(3, GL_FLOAT, 0, VertexArray);
   glColorPointer(3, GL_FLOAT, 0, ColorArray);
   glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_BYTE, IndexArray);
}
