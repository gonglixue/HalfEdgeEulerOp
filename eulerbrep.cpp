﻿#include "eulerbrep.h"

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

// 给定点是有顺序的？
Loop* EulerBrep::mef(float x1, float y1, float z1,
                     float x2, float y2, float z2,
                     Loop *loop)
{
    Solid* solid = loop->face_->solid_;
    Vertex* v1 = solid->FindVertex(x1, y1, z1);
    Vertex* v2 = solid->FindVertex(x2, y2, z2);
    if(!v1 || !v2)
    {
        std::cout << "Error in EulerBrep::mef: thie given vertex doesn't exit\n";
        exit(1);
    }
    // 判断给定是否在给定环上
    if(!loop->VertexInLoop(v1) || !loop->VertexInLoop(v2) )
    {
        std::cout << "Error in EulerBrep::mef: the given vertex is not in ght given loop\n";
        exit(1);
    }

    Face* new_face = new Face; // 所生成的新面
    Loop* inner_loop = new Loop;
    Edge* new_edge;
    HalfEdge* he1 = new HalfEdge;
    HalfEdge* he2 = new HalfEdge;
    he1->SetStartV(v1);
    he2->SetStartV(v2);

    // 在给定环上分别找到以v1 v2为起点的边
    HalfEdge* v1_he = loop->FindHalfEdge(v1);
    HalfEdge* v2_he = loop->FindHalfEdge(v2);

    // 把给定loop上已存在的内圈halfedge的成员环，更新为新的环
    // ？？怎么保证所更新的环是内圈的halfedge？这和给定点的顺序有关吧？
    HalfEdge* temp_he = v1_he;
    while(temp_he)
    {
        if(temp_he == v2_he)
            break;
        temp_he->loop_ = inner_loop;
        temp_he = temp_he->next_he_;
    }

    // 把新的halfedge he1 he2加入对应的halfedge链表中
    // 把he2加入内环中
    temp_he = v1_he->prev_he_;
    he2->next_he_ = v1_he;
    he2->prev_he_ = v2_he->prev_he_;
    v2_he->prev_he_->next_he_ = he2;
    v1_he->prev_he_ = he2;
    inner_loop->AddHalfEdge(he2); // 此前inner_loop的halfedge链表是空的。he2已经在内环对应的halfedge链表中。这一行可代替下面的两行
    //he2->loop_ = inner_loop;
    //inner_loop->halfedges_ = he2;   // 可以这样吗？
    // 把he1加入外环中
    he1->prev_he_ = temp_he;
    temp_he->next_he_ = he1;
    he1->next_he_ = v2_he;
    v2_he->prev_he_ = he1;
    he1->loop_ = loop;  // 原先给定的loop作为外环，he1在外环上
    loop->halfedges_ = he1;

    // 构建物理边
    he1->phy_e_ = he2->phy_e_ = new_edge;
    new_edge->he1_ = he1;
    new_edge->he2_ = he2;
    he1->sym_he_ = he2;
    he2->sym_he_ = he1;

    new_face->AddLoop(inner_loop);  // 一个loop对应一个面？？？
    solid->AddFace(new_face);

    return inner_loop;

}
