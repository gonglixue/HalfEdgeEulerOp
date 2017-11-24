﻿#ifndef MYFIXEDGLWIDGET_H
#define MYFIXEDGLWIDGET_H

#include <QGLWidget>
#include <QString>
#include <GL/gl.h>
#include <GL/glu.h>
#include "eulerbrep.h"

class MyFixedGLWidget:public QGLWidget
{
    Q_OBJECT
public:
    explicit MyFixedGLWidget(QWidget *parent = 0);
    void ConstructBrep(QString brep_file_path);
    ~MyFixedGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    void draw();
    QPoint last_pos_;

    EulerBrep brep_;
    GLUtesselator* tess_obj_;

    // glutess callback
    //void CALLBACK vertexCallback(GLvoid *vertex);
    //void CALLBACK beginCallback(GLenum which);
    //void CALLBACK endCallback(void);
    //void CALLBACK errorCallback(GLenum which);

    float x_rot_;
    float y_rot_;
    float z_rot_;
};

#endif // MYFIXEDGLWIDGET_H