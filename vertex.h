#ifndef VERTEX_H
#define VERTEX_H
#define EPS 0.00001

#include <QVector3D>

class Vertex
{
public:
    Vertex(float x, float y, float z);
    void SetPosition(float x, float y, float z);
    bool EqualTo(float x, float y, float z);

    int vertex_id_;
    QVector3D position_;

    Vertex* next_v_;
    Vertex* prev_v_;
};

#endif // VERTEX_H
