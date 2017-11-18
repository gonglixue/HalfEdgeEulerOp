#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QPoint>
#include <qevent.h>
#include <QVector4D>
#include <QString>
#include "eulerbrep.h"
#include "mycamera.h"
#include "trimesh.h"

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    void ConstructBrep(QString brep_file_path);
    TriMesh mesh_;
signals:

public slots:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;


private:
    EulerBrep euler_;

    // transformation matrix
    MyCamera camera_;
    QMatrix4x4 model_;
    QMatrix4x4 view_;
    QMatrix4x4 projection_;

    // Shader Config
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO;
    QOpenGLBuffer EBO;
    QOpenGLShaderProgram *shader;
    GLuint model_mat_loc_;
    GLuint view_mat_loc_;
    GLuint projection_mat_loc_;

    void SetupVertexAttribs();

};

#endif // MYOPENGLWIDGET_H
