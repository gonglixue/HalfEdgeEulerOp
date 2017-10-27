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
};

#endif // EULERBREP_H
