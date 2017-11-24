#include <QtWidgets>
#include <QtOpenGL>

#include "myfixedglwidget.h"

MyFixedGLWidget::MyFixedGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}

MyFixedGLWidget::~MyFixedGLWidget()
{

}

QSize MyFixedGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyFixedGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void MyFixedGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 光源位置
    static GLfloat light_position[4] = {0, 0, 10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

// 绘制函数
void MyFixedGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //glTranslatef()
    //glRotatef();
    draw();
}

void MyFixedGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width- side)/2, (height-side)/2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void MyFixedGLWidget::mousePressEvent(QMouseEvent *event)
{
    last_pos_ = event->pos();
}

void MyFixedGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - last_pos_.x();
    int dy = event->y() - last_pos_.y();
    if(event->buttons() & Qt::LeftButton){

    }


    last_pos_ = event->pos();
}

void MyFixedGLWidget::draw()
{
    qglColor(Qt::red);
    // begin
}
