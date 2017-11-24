#include <QtWidgets>
#include <QtOpenGL>

#include "myfixedglwidget.h"

// callback
void CALLBACK vertexCallback(GLvoid* vertex)
{
    const GLdouble* pointer;
    pointer = (GLdouble *)vertex;
    glVertex3dv(pointer);
}

void CALLBACK endCallback()
{
    glEnd();
}

void CALLBACK beginCallback(GLenum which)
{
    glBegin(which);
}

void CALLBACK errorCallback(GLenum error_code)
{
    const GLubyte *error_string;
    error_string = gluErrorString(error_code);
    fprintf(stderr, "Tessellation Error: %s\n", error_string);
    exit(0);
}

MyFixedGLWidget::MyFixedGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      x_rot_(0),
      y_rot_(0),
      z_rot_(0)

{
    tess_obj_ = gluNewTess();
    ConstructBrep("");
    qDebug() << "MyFixedGLWidget::Constructor";
}

MyFixedGLWidget::~MyFixedGLWidget()
{

}

void MyFixedGLWidget::ConstructBrep(QString brep_file_path)
{
#ifndef DIRECT_TEST
    brep_.Test();
#endif
}

QSize MyFixedGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyFixedGLWidget::sizeHint() const
{
    return QSize(512, 512);
}

void MyFixedGLWidget::initializeGL()
{
    qglClearColor(Qt::black);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 光源位置
    static GLfloat light_position[4] = {0, 0, 10, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 初始化tessa
    gluTessCallback(tess_obj_, GLU_TESS_VERTEX, (void(CALLBACK *)())vertexCallback);
    gluTessCallback(tess_obj_, GLU_TESS_BEGIN, (void(CALLBACK *)())beginCallback);
    gluTessCallback(tess_obj_, GLU_TESS_END, (void(CALLBACK *)())endCallback);
    gluTessCallback(tess_obj_, GLU_TESS_ERROR, (void(CALLBACK *)())errorCallback);

}

// 绘制函数
void MyFixedGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFrontFace(GL_CCW); // 逆时针顶点顺序为正面

    glLoadIdentity();

    glPushMatrix();
    glRotatef(x_rot_, 1.0f, 0.0f, 0.0f);
    glRotatef(y_rot_, 1.0f, 0.0f, 0.0f);
    glRotatef(z_rot_, 1.0f, 0.0f, 0.0f);
    glPopMatrix();

    draw();

    glFlush();

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
    glLoadIdentity();
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
    Face* faces = brep_.brep_solid_->faces_;
    int index = 0;
    if(!faces)
    {
        std::cout << "MyFixedGLWidget::draw(): there is no faces to draw\n";
    }

    // begin
    while (faces) {
        gluTessBeginPolygon(tess_obj_, NULL);
        Loop* temp_loop = faces->loop_;

        // 遍历loop上的每个点
        while(temp_loop)
        {
            HalfEdge* half_edge = temp_loop->halfedges_;
            Vertex* start_v = half_edge->start_v_;
            gluTessBeginContour(tess_obj_);
            gluTessVertex(tess_obj_, start_v->pos_dp_, start_v->pos_dp_);
            Vertex* temp_vertex = half_edge->next_he_->start_v_;
            half_edge = half_edge->next_he_;
            while(temp_vertex != start_v)
            {
                gluTessVertex(tess_obj_, temp_vertex->pos_dp_, temp_vertex->pos_dp_);  // 参数不能是临时变量的指针
                half_edge = half_edge->next_he_;
                temp_vertex = half_edge->start_v_;
            }
            gluTessEndContour(tess_obj_);
            temp_loop = temp_loop->next_loop_;
        }
        gluTessEndPolygon(tess_obj_); // 结束绘制一个面
        faces = faces->next_face_;
        index++;
    }
}


