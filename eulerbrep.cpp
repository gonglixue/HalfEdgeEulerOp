#include "eulerbrep.h"

EulerBrep::EulerBrep()
{

}

Solid* EulerBrep::mvfs(float x, float y, float z)
{
    Solid* solid = new Solid;
    Loop* loop = new Loop;
    Face* face = new Face;
    Vertex* vertex = new Vertex(x, y, z);

    face->AddLoop(loop);
    solid->AddFace(face);

    solid->AddVertex(vertex);

    return solid;
}

HalfEdge* EulerBrep::mev(float x1, float y1, float z1,
                         float x2, float y2, float z2,
                         Loop *loop)
{
    Solid* solid = loop->face_->solid_;
    Edge* edge = new Edge;
    Vertex* new_vertex = new Vertex(x2, y2, z2);

    HalfEdge* he1 = new HalfEdge;
    HalfEdge* he2 = new HalfEdge;

    Vertex* given_vertex = solid->FindVertex(x1, y1, z1);
    if(!given_vertex)
    {
        std::cout << "Error in EulerBrep::mev: thie given vertex doesn't exit\n";
        exit(1);
    }

    // 找到了给定点（x1, y1, z1)
    // 构造halfedge
    he1->SetStartV(given_vertex);   // 以给定点为起点
    he2->SetStartV(new_vertex);     // 以新点为起点
    he1->sym_he_ = he2;
    he2->sym_he_ = he1;

    he1->loop_ = he2->loop_ = loop;

    //he1->next_he_ = he2;
    //he2->prev_he_ = he1;

    // 构造物理边Edge
    he1->phy_e_ = he2->phy_e_ = edge;
    edge->he1_ = he1;
    edge->he2_ = he2;

    // 把边添加到给定环上
    HalfEdge* temp_he = loop->halfedges_;
    if(!temp_he)
    {
        // he1与he2自己构成一个环
        he1->next_he_ = he2;
        he2->next_he_ = he1;
        he1->prev_he_ = he2;
        he2->prev_he_ = he1;
        loop->halfedges_ = he1;
    }
    else
    {
        // 找到以给定点为终点的half edge
        while(temp_he->next_he_->start_v_ != given_vertex)
            temp_he = temp_he->next_he_;
        // 连接到half edge构成的环中
        he2->next_he_ = temp_he->next_he_;
        temp_he->next_he_->prev_he_ = he2;
        temp_he->next_he_ = he1;
        he1->prev_he_ = temp_he;
        he1->next_he_ = he2;
        he2->prev_he_ = he1;
    }

    solid->AddEdge(edge);
    solid->AddVertex(vertex);

    return he1;

}
