#ifndef MODEL_H
#define MODEL_H
#include <QVector3D>

struct Solid;
struct Face;
struct Loop;
struct Edge;
struct HalfEdge;
struct Vertex;

// Solid
struct Solid
{
    int solid_id_;

    Face *faces_; // all the faces belong to this slid
    //Loop *loops_;
    Vertex *vertices_;
    Edge *edges_;

    Solid *next_solid_;
    Solid *prev_solid_;

    int num_vertex_;
    int num_face_;
    int num_edge_;

    Solid(){
        static int id = 0;
        this->solid_id_ = id++;

        num_vertex_ = num_face_ = num_edge_ = 0;
        faces_ = vertices_ = edges_ = NULL;
        next_solid_ = prev_solid_ = NULL;
    }

    // 添加面
    bool AddFace(Face* new_face)
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
    bool AddEdge(Edge* new_edge)
    {
        if(!new_edge)
            return false;

        if(!this->edges_)
            this->edges_ = new_dege;
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
    bool AddVertex(Vertex* new_vertex)
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
    Edge* FindEdge(Vertex* v1, Vertex* v2)
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

};

// Loop
struct Loop
{
    int loop_id_;
    HalfEdge *halfedges_;
    Face *face_;  // the face that constructed by this loop, only one?

    Loop *next_loop_;
    Loop *pre_loop_;

    Loop():halfedges_(NULL), face_(NULL), next_loop_(NULL),
        pre_loop_(NULL){
        static int id = 0;
        loop_id_ = id++;
    }

    // 添加half edge 最后怎么闭合的？
    bool AddHalfEdge(HalfEdge* he)
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
    bool VertexInLoop(Vertex* vertex)
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
    HalfEdge* FindHalfEdge(Vertex* start_v)
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
};

// Face
struct Face
{
    int face_id_;
    Solid *solid_; // the solid this face belongs to? neccessary?

    Face* prev_face_;
    Face* next_face_;

    //Loop* outer_loop_;
    //Loop* inner_loop_;
    Loop* loop_;

    Face():prev_face_(NULL), next_face_(NULL), solid_(NULL), loop_(NULL)
    {
        static int id = 0;
        face_id_ = id++;
    }

    bool AddLoop(Loop* new_loop)
    {
        if(new_loop == NULL)
            return false;

        new_loop->face_ = this;
        if(loop_ == NULL)
        {
            // 当前面不包含环？
            this->loop_ = new_loop;
        }
        else{
            Loop* temp_loop = this->loop_;
            // 加到loop_链表中
            while(temp_loop->next_loop_)
            {
                temp_loop = temp_loop->next_loop_;
            }
            temp_loop->next_loop_ = new_loop;
            new_loop->pre_loop_ = temp_loop;
        }
        return true;
    }

};

// HalfEdge
struct HalfEdge
{
    Loop* loop_;
    Edge* phy_e_;

    HalfEdge* next_he_;
    HalfEdge* prev_he_;
    HalfEdge* sym_he_;

    Vertex* start_v_;

    int half_edge_id_;

    HalfEdge()
    {
        static int id = 0;
        half_edge_id_ = id++;

        next_he_ = prev_he_ = NULL;
        sym_he_ = NULL;
        loop_ = NULL;
        phy_e_ = NULL;
        start_v_ = NULL;
    }

    void SetStartV(Vertex* vertex)
    {
        start_v_ = vertex;
    }
};

// Vertex
struct Vertex
{
    int vertex_id_;
    QVector3D position_;

    Vertex* next_v_;
    Vertex* prev_v_;

    Vertex(float x, float y, float z):next_v_(NULL), prev_v_(NULL)
    {
        position_ = QVector3D(x,y,z);
    }
    void SetPosition(float x, float y, float z)
    {
        position_ = QVector3D(x,y,z);
    }
};

// Edge
struct Edge
{
    HalfEdge* he1_;
    HalfEdge* he2_;  // he2 = he1->sym_he

    Edge* prev_edge_;
    Edge* next_edge_;

    int edge_id_;

    Edge()
    {
        static int id = 0;
        edge_id_ = id++;
        prev_edge_ = next_edge_ = NULL;
        he1_ = he2_ = NULL;
    }
};

#endif // MODEL_H
