#include "eulerbrep.h"
#include <QStringList>
#include <QTextStream>

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
Solid* EulerBrep::mvfs(QVector3D &v)
{
    return mvfs(v.x(), v.y(), v.z());
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
    solid->AddVertex(new_vertex);

    std::cout << "success make he: given("<<x1<<","<<y1<<","<<z1<<")"<<" and new ("<<x2<<","<<y2<<","<<z2<<")\n";
    return he1;

}

HalfEdge* EulerBrep::mev(const QVector3D &p1, const QVector3D &p2, Loop *loop)
{
    return mev(p1.x(), p1.y(), p1.z(),
        p2.x(), p2.y(), p2.z(),
        loop);
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
    } //找以v2为终点的边

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

    new_face->AddLoop(inner_loop);  //??
    // 此前是没有face的？
    solid->AddFace(new_face);

    return inner_loop;

}

Loop* EulerBrep::mef(QVector3D &v1, QVector3D &v2, Loop *loop)
{
    return mef(v1.x(), v1.y(), v1.z(),
               v2.x(), v2.y(), v2.z(),
               loop);
}

Loop* EulerBrep::kemr(HalfEdge *bridge_he1, Loop *loop)
{
    HalfEdge* bridge_he2 = bridge_he1->sym_he_;
    Loop* new_loop = new Loop; //所产生的新环

    // 如果给定loop上只有he1 he2，是不能删除的它的
    // ...

    if(bridge_he1->next_he_ != bridge_he2){
        // 把内边界的halfedge加到新环里
        new_loop->AddHalfEdge(bridge_he1->next_he_);
        HalfEdge* temp_he = bridge_he1->next_he_->next_he_;
        while(temp_he)
        {
            if(temp_he == bridge_he2)
                break;
            temp_he->loop_ = new_loop;
            temp_he = temp_he->next_he_;
        }
        bridge_he1->next_he_->prev_he_ = bridge_he1->prev_he_;
        bridge_he2->prev_he_->next_he_ = bridge_he1->next_he_;
    }

    // 面添加一个inner loop
    loop->face_->AddLoop(new_loop);
    loop->halfedges_ = bridge_he1->prev_he_;

    // 重新闭合外环
    bridge_he1->prev_he_->next_he_ = bridge_he2->next_he_;
    bridge_he2->next_he_->prev_he_ = bridge_he1->prev_he_;

    Solid* solid = loop->face_->solid_;
    Edge* edge = solid->edges_;
    // 找到给定半边对应的物理边
    while(edge)
    {
        if(edge->he1_==bridge_he1 || edge->he2_==bridge_he2)
            break;
        edge = edge->next_edge_;
    }
    // 下面删除这条物理边
    // 如果给定的边是链表的中间节点
    if(edge->next_edge_ && edge->prev_edge_)
    {
        edge->next_edge_->prev_edge_ = edge->prev_edge_;
        edge->prev_edge_->next_edge_ = edge->next_edge_;
    }
    // 如果给定边是最后一个节点
    else if(!edge->next_edge_ && edge->prev_edge_)
    {
        edge->prev_edge_->next_edge_ = NULL;
    }
    else if(edge->next_edge_ && !edge->prev_edge_)
    {
        edge->next_edge_->prev_edge_ = NULL;
        solid->edges_ = edge->next_edge_;
    }
    // 只有该边（可能发生吗？）
    else{
        solid->edges_ = NULL;
    }

    delete edge;
    delete bridge_he1;
    delete bridge_he2;

    return new_loop;
}

Loop* EulerBrep::kemr(float x1, float y1, float z1, float x2, float y2, float z2, Loop *loop)
{
    Solid* solid = loop->face_->solid_;
    Vertex* v1 = solid->FindVertex(x1, y1, z1);
    Vertex* v2 = solid->FindVertex(x2, y2, z2);
    if(!v1 || !v2)
    {
        std::cout<< "Error in EulerBrep::Kemr(2)::can not find the given point\n";
        exit(1);
    }
    // 给定的点要在同一环上 ？ 这个判断必要吗？
    // 找到要删除的那个桥边(v1为起点，v2为终点）
    HalfEdge *he = loop->halfedges_;
    while(he)
    {
        if(he->start_v_==v1 && he->next_he_->start_v_==v2)
        {
            break;
        }
        he = he->next_he_;
    }
    if(!he)
    {
        std::cout<<"Error in EulerBrep::kemr(2)::can not find bridge half edge with given point\n";
        exit(1);
    }
    else{
        kemr(he, loop);
    }
}

Loop* EulerBrep::kemr(QVector3D &v1, QVector3D &v2, Loop *loop)
{
    return kemr(v1.x(), v1.y(), v1.z(),
                v2.x(), v2.y(), v2.z(),
                loop);
}

// 删除一个面，把内环加到外环所在的面上
void EulerBrep::kfmrh(Loop *outter_loop, Loop *inner_loop)
{
    Face *temp_face = inner_loop->face_;  //准备删掉的面

    // 把内环加到外环所在的面上
    outter_loop->face_->AddLoop(inner_loop);
    // 在面链表中删去temp_face
    if(temp_face->next_face_ && temp_face->prev_face_){
        temp_face->prev_face_->next_face_ = temp_face->next_face_;
        temp_face->next_face_->prev_face_ = temp_face->prev_face_;
    }
    else if(temp_face->next_face_==NULL && temp_face->prev_face_!=NULL){
        // 是尾节点
        temp_face->prev_face_->next_face_ = NULL;
    }
    else if(temp_face->prev_face_==NULL && temp_face->next_face_!=NULL)
    {
        temp_face->next_face_->prev_face_ = NULL;
        outter_loop->face_->solid_->faces_ = temp_face->next_face_;
    }
    else
        outter_loop->face_->solid_->faces_ = NULL;

    delete temp_face;
    return;
}

//  x, y, z是扫成的方向偏量
void EulerBrep::sweep(Face* face, float x, float y, float z)
{
    QVector3D v1, v2, first_up, prev_up;//prev_up是上一个扫成得到的点
    for(Loop* temp_loop = face->loop_; temp_loop!=NULL; temp_loop=temp_loop->next_loop_)
    {
        // 记录当前半边，和前后两个半边
        HalfEdge *temp_he = temp_loop->halfedges_; //用来记录当前halfedge
        //HalfEdge *next_he = temp_he->next_he_;
        HalfEdge *prev_he = temp_he->prev_he_;  //记录第一条halfedge的上一条halfedge，用来判断该loop是否遍历结束
        Vertex* vertex = temp_he->start_v_;  // 当前半边的起点

        v1 = vertex->position_;
        first_up = prev_up = v2 = v1 + QVector3D(x, y, z);
        mev(v1, v2, temp_loop); //该loop上的第一个点的扫成

        if(temp_he == prev_he)
            temp_he = NULL;  // ？？ 说明这个环只有一对halfedge
        else{
            //temp_he = next_he;
            //next_he = temp_he->next_he_;
            temp_he = temp_he->next_he_;
        }

        while(temp_he)  // 对环上的每个half edge的起点进行偏移
        {
            Vertex* vertex = temp_he->start_v_;
            v1 = vertex->position_;        // 原来的点坐标;当前需要操作的点
            v2 = v1 + QVector3D(x, y, z);  // 平移后的点

            mev(v1, v2, temp_loop);  // 在原来的点和新的点之间连一条边
            mef(prev_up, v2, temp_loop);  // last_up是上一个v1平移后的点
            prev_up = v2;

            if(temp_he == prev_he)  //最后一条halfedge
            {
                mef(prev_up, first_up, temp_loop);
                break;
            }
            else{
                //temp_he = next_he;
                //next_he = temp_he->next_he_;
                temp_he = temp_he->next_he_;
            }
        }
    }
}

// 立方体
void EulerBrep::Test()
{
    QVector3D vertices_outer[4] = {QVector3D(0.0, 0.0, 0.0), QVector3D(3.0, 0.0, 0.0), QVector3D(3.0, 3.0, 0.0), QVector3D(0.0, 3.0, 0.0)};
    QVector3D vertices_inner[4] = {QVector3D(1.0, 1.0, 0.0), QVector3D(2.0, 1.0, 0.0), QVector3D(2.0, 2.0, 0.0), QVector3D(1.0, 2.0, 0.0)};
    QVector3D cube_vertices[8] = {
        QVector3D(0,0,0),
        QVector3D(2,0,0),
        QVector3D(2,2,0),
        QVector3D(0,2,0),
        QVector3D(0,0,2),
        QVector3D(2,0,2),
        QVector3D(2,2,2),
        QVector3D(0,2,2)
    };
    // 创建第一个点, 以及生成一个大环，这个大环在一开始时并无特殊含义. mef之后产生的环才是组成这个面的环
    brep_solid_ = this->mvfs(vertices_outer[0]);
    Loop* big_loop = brep_solid_->faces_->loop_;
    // 后面
    mev(cube_vertices[0], cube_vertices[1], big_loop);
    mev(cube_vertices[1], cube_vertices[2], big_loop);
    mev(cube_vertices[2], cube_vertices[3], big_loop);
    mef(cube_vertices[3], cube_vertices[0], big_loop);
    // 底面
    mev(cube_vertices[0], cube_vertices[4], big_loop);
    mev(cube_vertices[1], cube_vertices[5], big_loop);
    mef(cube_vertices[4], cube_vertices[5], big_loop);
    // right
    mev(cube_vertices[2], cube_vertices[6], big_loop);
    mef(cube_vertices[5], cube_vertices[6], big_loop);
    // left
    mev(cube_vertices[3], cube_vertices[7], big_loop);
    mef(cube_vertices[6], cube_vertices[7], big_loop);

    mef(cube_vertices[7], cube_vertices[4], big_loop);

    std::cout << "brep::Test() finish.\n";
}

void EulerBrep::TestWithTwoHandle()
{
    QVector3D vertex_outter[4] = {
        QVector3D(0, 0, 0),  // 0
        QVector3D(3, 0, 0),  // 1
        QVector3D(3, 5, 0),  // 2
        QVector3D(0, 5, 0)   // 3
    };//顶面的4个外顶点
    QVector3D vertex_inner_1[4] = {
        QVector3D(1, 1, 0),  // 4
        QVector3D(2, 1, 0), // 5
        QVector3D(2, 2, 0), // 6
        QVector3D(1, 2, 0)  // 7
    };  //第一个内环的四个顶点
    QVector3D vertex_inner_2[4] = {
        QVector3D(1, 3, 0), // 8
        QVector3D(2, 3, 0), // 9
        QVector3D(2, 4, 0), // 10
        QVector3D(1, 4, 0)  // 11
    };

    brep_solid_  = mvfs(vertex_outter[0]);
    Loop* big_loop = brep_solid_->faces_->loop_;

    // 生成顶面外边界
    mev(vertex_outter[0], vertex_outter[1], big_loop);
    mev(vertex_outter[1], vertex_outter[2], big_loop);
    mev(vertex_outter[2], vertex_outter[3], big_loop);
    mef(vertex_outter[3], vertex_outter[0], big_loop);

    // 此时存在两个face, big_loop在第一个face上，mef产生的完整的loop在第二个face上
    // 第一个柄
    Loop* temp_loop = brep_solid_->faces_->next_face_->loop_;  //顶面上的外环
    mev(vertex_outter[0], vertex_inner_1[0], temp_loop);
    temp_loop = kemr(vertex_outter[0], vertex_inner_1[0], temp_loop);  //temp_loop指向内环,但并没有和某个面建立关联
    mev(vertex_inner_1[0], vertex_inner_1[1], temp_loop);
    mev(vertex_inner_1[1], vertex_inner_1[2], temp_loop);
    mev(vertex_inner_1[2], vertex_inner_1[3], temp_loop);

    Loop* inner_loop = mef(vertex_inner_1[3], vertex_inner_1[0], temp_loop);// mef产生的环才是mef产生的新面上的环
    kfmrh(big_loop, inner_loop); //?

    //生成第二个柄
    temp_loop = brep_solid_->faces_->next_face_->loop_;  //顶面上的外环
    mev(vertex_outter[0], vertex_inner_2[0], temp_loop);
    temp_loop = kemr(vertex_outter[0], vertex_inner_2[0], temp_loop);
    mev(vertex_inner_2[0], vertex_inner_2[1], temp_loop);
    mev(vertex_inner_2[1], vertex_inner_2[2], temp_loop);
    mev(vertex_inner_2[2], vertex_inner_2[3], temp_loop);

    Loop* inner_loop2 = mef(vertex_inner_2[3], vertex_inner_2[0], temp_loop);
    kfmrh(big_loop, inner_loop2);

    // 扫成
    float dx = 0, dy = 0, dz = 3;
    sweep(brep_solid_->faces_, dx, dy, dz);

}

void EulerBrep::ReadBrepFromFile(QString fn)
{
    QFile file(fn);
    if(!file.open(QIODevice::ReadOnly))
        exit(-1);

    QTextStream in(&file);
    QString line;
    std::vector<QVector3D> vertices_list;
    int vert_num;
    float x, y, z;

    Loop* big_loop;
    Loop* temp_loop;
    Loop* temp_mef_loop;

    float sum_x=0, sum_y=0, sum_z=0;
    float min_x=1000, min_y=1000, min_z=1000;
    float max_x=-1000, max_y=-1000, max_z=-1000;
    int count_v = 0;
    float length = 1.0;

    while(!in.atEnd())
    {
        line = in.readLine();
        if(line.isEmpty()){
            continue;
        }
        if(line[0] == '#')
            qDebug() << line;
        else if(line[0] == 'n')
        {
            line.remove(0, 2); // remove "n "
            vert_num = line.toInt();
            qDebug() << "vertex num: " << vert_num;
        }
        else if(line[0] == 'v')
        {
            line.remove(0, 2); // remove "v "
            QStringList vertex_coords = line.split(' ', QString::SkipEmptyParts);
            x = vertex_coords[0].toFloat();
            y = vertex_coords[1].toFloat();
            z = vertex_coords[2].toFloat();
            vertices_list.push_back(QVector3D(x, y, z));

            sum_x+=x;
            sum_y+=y;
            sum_z+=z;

            if(x>max_z) max_x = x;
            if(y>max_y) max_y = y;
            if(z>max_z) max_z = z;

            if(x<min_x) min_x = x;
            if(y<min_y) min_y = y;
            if(z<min_z) min_z = z;

            count_v++;
            if(count_v == vert_num)
            {
                length = max_y - min_y;
                QVector3D center_coord = QVector3D(sum_x/vert_num, sum_y/vert_num, sum_z/vert_num);
                for(int i=0;i<vert_num;i++)
                {
                    vertices_list[i] = (vertices_list[i]-center_coord)/length;
                }
            }

        }
        else if(line[0] == 'h')
        {
            QStringList command = line.split(' ', QString::SkipEmptyParts);
            if(command[1] == "handle")
            {
                qDebug() << "begin to construct handle " << command[2];
                temp_loop = brep_solid_->faces_->next_face_->loop_;
            }
            else if(command[1] == "sweep"){
                qDebug() << "begin to sweep " << command[2];

            }
        }
        else if(line[0] == 'e')
        {
            line.remove(0, 2);  // remove "e_"
            QStringList euler_command = line.split(' ', QString::SkipEmptyParts);
            //qDebug() << "test command[0]:" << euler_command[0];
            if(euler_command[0] == "mvfs"){
                int ind = euler_command[1].toInt();
                brep_solid_ = mvfs(vertices_list[ind]);
                big_loop = brep_solid_->faces_->loop_;
                temp_loop = big_loop;
                qDebug() << "mvfs: " << vertices_list[ind];
            }
            else if(euler_command[0] == "mev"){
                int v1_ind = euler_command[1].toInt();
                int v2_ind = euler_command[2].toInt();
                mev(vertices_list[v1_ind], vertices_list[v2_ind], temp_loop);
                qDebug() << "mev: " << vertices_list[v1_ind] << " " << vertices_list[v2_ind] << " in loop: " << temp_loop->loop_id_;

            }
            else if(euler_command[0] == "kemr"){
                int v1_ind = euler_command[1].toInt();
                int v2_ind = euler_command[2].toInt();
                temp_loop = kemr(vertices_list[v1_ind], vertices_list[v2_ind], temp_loop);

            }
            else if(euler_command[0] ==  "mef"){
                int v1_ind = euler_command[1].toInt();
                int v2_ind = euler_command[2].toInt();
                temp_mef_loop = mef(vertices_list[v1_ind], vertices_list[v2_ind], temp_loop);
                qDebug() << "mef: " << vertices_list[v1_ind] << " " << vertices_list[v2_ind] << " in loop: " << temp_loop->loop_id_;

            }
            else if(euler_command[0] ==  "kfmrh"){
                qDebug() << "kfmrh: " << big_loop->loop_id_ << " " << temp_mef_loop->loop_id_;
                kfmrh(big_loop, temp_mef_loop);
            }
            else if(euler_command[0] ==  "sweep"){
                float dx = euler_command[1].toFloat();
                float dy = euler_command[2].toFloat();
                float dz = euler_command[3].toFloat();
                sweep(brep_solid_->faces_, dx/length, dy/length, dz/length);
            }
            else
                qDebug() << "illegal euler command.";
        }

        else{
            qDebug() << "illegal euler command.";
        }
    }
    file.close();
}

void EulerBrep::clean()
{

}
