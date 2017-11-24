#ifndef MYFIXEDGLWIDGET_H
#define MYFIXEDGLWIDGET_H

#include <QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>

class MyFixedGLWidget:public QGLWidget
{
    Q_OBJECT
public:
    explicit MyFixedGLWidget(QWidget *parent = 0);
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

};

#endif // MYFIXEDGLWIDGET_H
