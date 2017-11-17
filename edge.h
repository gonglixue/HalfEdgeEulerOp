#ifndef EDGE_H
#define EDGE_H
#include "halfedge.h"
#include "face.h"
#include "loop.h"
#include "vertex.h"
class HalfEdge;

class Edge
{
public:
    Edge();


    HalfEdge* he1_;
    HalfEdge* he2_;  // he2 = he1->sym_he

    Edge* prev_edge_;
    Edge* next_edge_;

    int edge_id_;
};

#endif // EDGE_H
