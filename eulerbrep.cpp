#include "eulerbrep.h"

EulerBrep::EulerBrep()
{

}

Solid* EulerBrep::mvfs(float x, float y, float z)
{
   // construct a new solid
    this->brep_solid_ = new Solid();

    // construct a new vertex, a new loop, a new face, a new solid
    Vertex* new_v = new Vertex(x,y,z);
    Loop* new_loop = new Loop();
    Face* new_face = new Face();

    new_v->vertex_id_ = this->brep_solid_->num_vertex_;
    new_loop->loop_id_ = this->brep_solid_->num_loop_;
    new_face->face_id_ = this->brep_solid_->num_face_;
    new_face->solid_ = this->brep_solid_;
    new_face->outer_loop_ = new_loop;
    new_loop->face_ = new_face;

    IncreaseSolidVertex();
    IncreaseSolidLoop();
    IncreaseSolidFace();


    this->vertices_.push_back(new_v);
    this->loops_.push_back(new_loop);

    this->brep_solid_->faces_ = new_face;



}
