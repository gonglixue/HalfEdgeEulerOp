#ifndef MODEL_H
#define MODEL_H
#include <QVector3D>

struct Solid;
struct Face;
struct Loop;
struct Edge;
struct HalfEdge;
struct Vertex;

struct Solid
{
    int solid_id;
    Face *faces; // all the faces belong to this slid

    Solid *next_solid;
    Solid *prev_solid;

    int num_vertex;
};

struct Face
{
    int face_id;
    Solid *solid; // the solid this face belongs to

    Face* prev_face;
    Face* next_face;

    Loop* loops;
};

struct HalfEdge
{
    Loop* loop;
    Edge* phy_e;

    HalfEdge* next_he;
    HalfEdge* prev_he;
    HalfEdge* sym_he;

    Vertex* start_v;

};

struct Vertex
{
    int vertex_id;
    QVector3D position;

    Vertex* next_v;
    Vertex* prev_v;

    Vertex(float x, float y, float z):next_v(NULL), prev_v(NULL)
    {
        position = QVector3D(x,y,z);
    }
};

struct Edge
{
    HalfEdge* he1;
    HalfEdge* he2;  // he2 = he1->sym_he

    Edge* prev_edge;
    Edge* next_edge;
};

#endif // MODEL_H
