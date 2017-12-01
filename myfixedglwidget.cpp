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

static void qNormalizeAngle(float &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

static QVector3D GetArcBallVector(int x_p, int y_p, int screen_width, int screen_height)
{
    // arcball
    // 把屏幕坐标转为[-1, 1]
    float x = 2*x_p/screen_width - 1;
    float y = -(2*y_p/screen_height - 1);

    QVector3D P = QVector3D(x, y, 0);
    float OP_squared = P.x()*P.x() + P.y()*P.y();
    if(OP_squared <= 1)
        P.setZ(sqrtf(1.0f - OP_squared));
    else
        P.normalize();

    return P;
}

MyFixedGLWidget::MyFixedGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
      x_rot_(0),
      y_rot_(0),
      z_rot_(0)

{
    tess_obj_ = gluNewTess();
    model_matrix_.setToIdentity();
    //model_matrix_.translate(1, -1, -3.0);
    qDebug() << "MyFixedGLWidget::Constructor";
    rot_degree_ = 0;
    rot_axis_ = QVector3D(1, 1, 1);
    camera_pos_ = -5;
}

MyFixedGLWidget::~MyFixedGLWidget()
{

}

void MyFixedGLWidget::ConstructBrep(QString brep_file_path)
{
#ifdef DIRECT_TEST
    brep_.TestWithTwoHandle();
#else
    brep_.ReadBrepFromFile(brep_file_path);
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

void MyFixedGLWidget::PrintBrep()
{
    if(!brep_.brep_solid_)
    {
        qDebug() << "PrintBrep()::brep_solid_ is empty.";
        return;
    }
    Face *faces = brep_.brep_solid_->faces_;
    if(!faces)
        qDebug() << "PrintBrep()::there is no faces do draw\n";
    int faces_count = 0;
    qDebug() << "Begin printing...";
    while(faces){
        Loop* temp_loop = faces->loop_;


        while(temp_loop)
        {
            qDebug() << "Face ("<<faces->face_id_<<") - Loop ("<<temp_loop->loop_id_<<")";
            HalfEdge* half_edge = temp_loop->halfedges_;
            Vertex* first_v = half_edge->start_v_;
            qDebug() << "   Vertex:("<<first_v->position_.x() << "," << first_v->position_.y() << ","<<first_v->position_.z()<<")";
            Vertex* temp_v = half_edge->next_he_->start_v_;
            half_edge = half_edge->next_he_;
            while(temp_v != first_v)
            {
                qDebug() << "   Vertex:("<< temp_v->position_.x() << "," << temp_v->position_.y() << ","<< temp_v->position_.z() <<")";
                half_edge = half_edge->next_he_;
                temp_v = half_edge->start_v_;
            }
            temp_loop = temp_loop->next_loop_;
        }
        faces = faces->next_face_;
        faces_count++;
    }
    qDebug() << "finish print " << faces_count << " faces.";
}

void MyFixedGLWidget::initializeGL()
{
#ifdef DIRECT_TEST
    ConstructBrep("");
    qDebug() << "finish construct brep";
    PrintBrep();
#endif

    glClearColor(0.6, 0.76, 0.78, 1.0);
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

    //glRotatef(rot_degree_, rot_axis_.x(), rot_axis_.y(), rot_axis_.z());

    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(x_rot_, 1, 0, 0);
    glRotatef(y_rot_, 0, 1, 0);
    glRotatef(z_rot_, 0, 0, 1);

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
    screen_height_ = height;
    screen_width_ = width;

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
    gluLookAt(0, 0, camera_pos_, 0, 0, 0, 0, 1, 0);

}

void MyFixedGLWidget::mousePressEvent(QMouseEvent *event)
{
    last_pos_ = event->pos();
    //qDebug() << "press: " << last_pos_;
}

void MyFixedGLWidget::mouseMoveEvent(QMouseEvent *event)
{

    // arcball
    if(event->buttons() & Qt::LeftButton){
//        QVector3D va = GetArcBallVector(last_pos_.x(), last_pos_.y(), 512, 512);
//        QVector3D vb = GetArcBallVector(event->x(), event->y(), 512, 512);

//        float angle = acosf(qMin(1.0f, QVector3D::dotProduct(va, vb)));
//        QVector3D axis_in_camera_coord = QVector3D::crossProduct(va, vb);
//        QMatrix4x4 camera2object = (view_matrix_* model_matrix_).inverted();

//        QVector4D axis_in_object_coord = camera2object * axis_in_camera_coord;
//        model_matrix_.rotate(qRadiansToDegrees(angle), axis_in_object_coord.toVector3D());
//        rot_degree_ = qRadiansToDegrees(angle);
//        rot_axis_ = axis_in_object_coord.toVector3D();
        //glMatrixMode(GL_MODELVIEW);
        //glRotatef(angle, axis_in_object_coord.x(), axis_in_object_coord.y(), axis_in_object_coord.z());

        int dx = event->x() - last_pos_.x();
        int dy = event->y() - last_pos_.y();
        if(event->buttons() & Qt::LeftButton){
            setXRotation(x_rot_ + 8*dy);
            setYRotation(y_rot_ + 8*dx);
        }

        //qDebug() << "move";
        last_pos_ = event->pos();
        updateGL();
    }
}

void MyFixedGLWidget::wheelEvent(QWheelEvent *event)
{
    float forward_offset = event->delta()/100.0f;
    camera_pos_ += forward_offset;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, camera_pos_, 0, 0, 0, 0, 1, 0);

    updateGL();
}

void MyFixedGLWidget::draw()
{
    if(brep_.brep_solid_){

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

    }  // if(brep_.brep_solid_)
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



