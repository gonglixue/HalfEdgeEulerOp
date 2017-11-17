#include "loop.h"

Loop::Loop():halfedges_(NULL), face_(NULL), next_loop_(NULL),
    pre_loop_(NULL){
    static int id = 0;
    loop_id_ = id++;
}

// 添加half edge 最后怎么闭合的？
bool Loop::AddHalfEdge(HalfEdge* he)
{
    if(he == NULL)
        return false;

    he->loop_ = this;
    if(this->halfedges_ == NULL)
        halfedges_ = he;
    else
    {
        HalfEdge* temp_he = this->halfedges_;
        while(temp_he->next_he_)
        {
            temp_he = temp_he->next_he_;
        }
        temp_he->next_he_ = he;
        he->prev_he_ = temp_he;
    }

    return true;
}

// 判断该点是否在环上
bool Loop::VertexInLoop(Vertex* vertex)
{
    if(!vertex)
        return false;

    HalfEdge *this_hes = this->halfedges_;
    while(this_hes)
    {
        if(this_hes->start_v_ == vertex)
            return true;
        this_hes = this_hes->next_he_;
        // 遍历所有边未找到
        if(this_hes == halfedges_)
            return false;
    }
    return true;
}

// 找到该点为起点的halfedge
HalfEdge* Loop::FindHalfEdge(Vertex* start_v)
{
    if(!start_v)
        return NULL;

    HalfEdge* temp_he = this->halfedges_;
    while(temp_he)
    {
        if(temp_he->start_v_ == start_v)
            break;

        temp_he = temp_he->next_he_;
        if(temp_he == this->halfedges_)
            return NULL;
    }
    return temp_he;  // 可能为空
}
