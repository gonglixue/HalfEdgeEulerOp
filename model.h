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
    int solid_id_;
    Face *faces_; // all the faces belong to this slid
    Loop *loops_;
    Vertex *vertices_;
    Edge *edges_;

    Solid *next_solid_;
    Solid *prev_solid_;

    int num_vertex_;
    int num_face_;
    int num_loop_;

    Solid(){
        num_vertex_ = num_face_ = num_loop_ = 0;
        faces_ = vertices_ = edges_ = NULL;
        loops_ = NULL;
    }
};

struct Loop
{
    int loop_id_;
    HalfEdge *halfedges_;
    Face *face_;  // the face that constructed by this loop, only one?

    Loop *next_loop_;
    Loop *pre_loop_;

    Loop():loop_id_(0), halfedges_(NULL), face_(NULL), next_loop_(NULL),
        pre_loop_(NULL){}
};

struct Face
{
    int face_id_;
    Solid *solid_; // the solid this face belongs to? neccessary?

    Face* prev_face_;
    Face* next_face_;

    Loop* outer_loop_;
    Loop* inner_loop_;

};

struct HalfEdge
{
    Loop* loop_;
    Edge* phy_e_;

    HalfEdge* next_he_;
    HalfEdge* prev_he_;
    HalfEdge* sym_he_;

    Vertex* start_v_;

};

struct Vertex
{
    int vertex_id_;
    QVector3D position_;

    Vertex* next_v_;
    Vertex* prev_v_;

    Vertex(float x, float y, float z):next_v_(NULL), prev_v_(NULL)
    {
        position_ = QVector3D(x,y,z);
        next_v_ = prev_v_ = NULL;
    }
};

struct Edge
{
    HalfEdge* he1_;
    HalfEdge* he2_;  // he2 = he1->sym_he

    Edge* prev_edge_;
    Edge* next_edge_;
};

#endif // MODEL_H
