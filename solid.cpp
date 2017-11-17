#include "solid.h"

Solid::Solid()
{
    static int id = 0;
    this->solid_id_ = id++;

    num_vertex_ = num_face_ = num_edge_ = 0;
    faces_ = NULL;
    vertices_ = NULL;
    edges_ = NULL;
    next_solid_ = prev_solid_ = NULL;
}

bool Solid::AddFace(Face *new_face)
{
    if(new_face == NULL)
        return false;

    new_face->solid_ = this;
    if(this->faces_ == NULL)
        this->faces_ = new_face;
    else
    {
        Face* temp_face = this->faces_;
        while(temp_face->next_face_)
            temp_face = temp_face->next_face_;

        temp_face->next_face_ = new_face;
        new_face->prev_face_ = temp_face;
    }
    return true;
}
// 添加边
bool Solid::AddEdge(Edge *new_edge)
{
    if(!new_edge)
        return false;

    if(!this->edges_)
        this->edges_ = new_edge;
    else
    {
        Edge* temp_edge;
        while(temp_edge->next_edge_)
            temp_edge = temp_edge->next_edge_;
        temp_edge->next_edge_ = new_edge;
        new_edge->prev_edge_ = temp_edge;
    }
    return true;
}

// 添加点
bool Solid::AddVertex(Vertex* new_vertex)
{
    if(!new_vertex)
        return false;

    if(!this->vertices_)
        this->vertices_ = new_vertex;
    else
    {
        Vertex* temp_vertex = this->vertices_;
        while(temp_vertex->next_v_)
            temp_vertex = temp_vertex->next_v_;

        temp_vertex->next_v_ = new_vertex;
        new_vertex->prev_v_ = temp_vertex;
    }
    return true;
}

// 找到以给定点为端点的边
Edge* Solid::FindEdge(Vertex* v1, Vertex* v2)
{
    Edge* temp_edge = this->edges_;
    while(temp_edge)
    {
        if(temp_edge->he1_->start_v_ == v1 &&
                temp_edge->he2_->start_v_ == v2)
            return temp_edge;

        temp_edge = temp_edge->next_edge_;
    }
    return temp_edge;
}

// 找到给定点
Vertex* Solid::FindVertex(float x, float y, float z)
{
    Vertex* temp_vertex = this->vertices_;
    while(!temp_vertex)
    {
        if(temp_vertex->EqualTo(x, y, z))
            return temp_vertex;

        temp_vertex = temp_vertex->next_v_;
    }
    return NULL;
}
