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
    qglClearColor(Qt::white);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // 光照模型
    GLfloat light_ambient [] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse [] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient );
    glLightfv(GL_LIGHT0, GL_DIFFUSE , light_diffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);



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
    //glFrontFace(GL_CW);

    //glLoadIdentity();

    glPushMatrix();
    glTranslatef(-1, -1, -3.0);
    glRotatef(x_rot_, 1.0f, 0.0f, 0.0f);
    glRotatef(y_rot_, 0.0f, 1.0f, 0.0f);
    glRotatef(z_rot_, 0.0f, 0.0f, 1.0f);

    // 光源位置
    static GLfloat light_position[4] = {2, 2, 2, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    draw();
    glPopMatrix();
    //glFlush();

}

void MyFixedGLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width- side)/2, (height-side)/2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef ORTHO_PROJECT
    glOrthof(-5, +5, -5, +5, 0.10, 15.0);
#else
    float aspect = (float)width/height;
    gluPerspective(45.0f, aspect, 0.1, 100);
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, -15, 0, 0, 0, 0, 1, 0);

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
        setXRotation(x_rot_ + 8*dy);
        setYRotation(y_rot_ + 8*dx);
    }


    last_pos_ = event->pos();
}

void MyFixedGLWidget::draw()
{
    //qglColor(Qt::red);
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
//            int rand_r = 255 * rand()/RAND_MAX;
//            int rand_g = 255 * rand()/RAND_MAX;
//            int rand_b = 255 * rand()/RAND_MAX;
//            glColor3b(rand_r, rand_g, rand_b);
            glColor3b(255, 0, 0);
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

void MyFixedGLWidget::drawTest()
{
    qDebug() << "draw";
    glColor3b(255, 0, 0);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
}

static void qNormalizeAngle(float &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void MyFixedGLWidget::setXRotation(float angle)
{
    qNormalizeAngle(angle);
    x_rot_ = angle;
    emit xRotationChanged(angle);
    updateGL();
}

void MyFixedGLWidget::setYRotation(float angle)
{
    qNormalizeAngle(angle);
    y_rot_ = angle;
    emit yRotationChanged(angle);
    updateGL();
}



