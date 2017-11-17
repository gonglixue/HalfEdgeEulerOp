#include "vertex.h"

Vertex::Vertex(float x, float y, float z):next_v_(NULL), prev_v_(NULL)
{
    position_ = QVector3D(x,y,z);
}

void Vertex::SetPosition(float x, float y, float z)
{
    position_ = QVector3D(x,y,z);
}
bool Vertex::EqualTo(float x, float y, float z)
{
    QVector3D comp_vert = QVector3D(x, y, z);
    float dist = position_.distanceToPoint(comp_vert);
    if(dist < EPS)
        return true;

    return false;
}
