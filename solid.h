#ifndef SOLID_H
#define SOLID_H
#include "face.h"
#include "halfedge.h"
#include "loop.h"
#include "vertex.h"
#include "edge.h"
class Face;
class Edge;
class Vertex;

class Solid
{
public:
    Solid();
    bool AddFace(Face* new_face);
    bool AddEdge(Edge* new_edge);
    bool AddVertex(Vertex* new_vertex);
    Edge* FindEdge(Vertex* v1, Vertex* v2);
    Vertex* FindVertex(float x, float y, float z);

    int solid_id_;

    Face *faces_; // all the faces belong to this slid
    //Loop *loops_;
    Vertex *vertices_;
    Edge *edges_;

    Solid *next_solid_;
    Solid *prev_solid_;

    int num_vertex_;
    int num_face_;
    int num_edge_;
};

#endif // SOLID_H
