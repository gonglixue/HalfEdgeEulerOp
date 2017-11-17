#ifndef LOOP_H
#define LOOP_H

#include "halfedge.h"
#include "face.h"
#include "vertex.h"
class HalfEdge;
class Vertex;
class Face;

class Loop
{
public:
    Loop();
    bool AddHalfEdge(HalfEdge* he);
    bool VertexInLoop(Vertex* vertex);
    HalfEdge* FindHalfEdge(Vertex* start_v);


    int loop_id_;
    HalfEdge *halfedges_;
    Face *face_;  // the face that constructed by this loop, only one?

    Loop *next_loop_;
    Loop *pre_loop_;
};

#endif // LOOP_H
