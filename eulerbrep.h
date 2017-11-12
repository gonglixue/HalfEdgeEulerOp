#ifndef EULERBREP_H
#define EULERBREP_H
#include <vector>
#include "model.h"
class EulerBrep
{
private:
    std::vector<Vertex *> vertices_;
    std::vector<Loop *> loops_;
    std::vector<Face *> faces_;
    Solid* brep_solid_;

    void IncreaseSolidVertex(){this->brep_solid_->num_vertex_++;}
    void IncreaseSolidFace(){this->brep_solid_->num_face_++;}
    void IncreaseSolidLoop(){this->brep_solid_->num_face_++;}
public:
    EulerBrep();
    Solid* mvfs(float x, float y, float z);
    HalfEdge* mev(float x1, float y1, float z1, float x2, float y2, float z2, Loop* loop);
    Loop* mef(float x1, float y1, float z1,
              float x2, float y2, float z2,
              Loop* loop);
    Loop* kemr(float x1, float y1, float z1,
               float x2, float y2, float z2,
               Loop* loop);
    Loop* kemr(HalfEdge* bridge_he, Loop* loop);
};

#endif // EULERBREP_H
