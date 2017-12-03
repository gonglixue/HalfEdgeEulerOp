#include "myopenglwidget.h"
#include <math.h>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    shader(0),
    VBO(QOpenGLBuffer::VertexBuffer),
    EBO(QOpenGLBuffer::IndexBuffer),
    camera_(QVector3D(0,0,5))
{


}

void MyOpenGLWidget::ConstructBrep(QString brep_file_path)
{
#ifdef DIRECT_TEST
    euler_.Test();

#endif
}
