#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "loop.h"
#include "edge.h"
#include "vertex.h"
class Loop;
class Edge;

class HalfEdge
{
public:
    HalfEdge();
    void SetStartV(Vertex* vertex)
    {
        start_v_ = vertex;
    }

    Loop* loop_;
    Edge* phy_e_;

    HalfEdge* next_he_;
    HalfEdge* prev_he_;
    HalfEdge* sym_he_;

    Vertex* start_v_;

    int half_edge_id_;
};

#endif // HALFEDGE_H
