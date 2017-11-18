#ifndef TRIMESH_H
#define TRIMESH_H
#include <vector>
#include <iostream>
#include <QVector3D>
class TriMesh
{
public:
    TriMesh();
    const QVector3D* Data() const {return vertices.data();}
    const int* Index() const{ return indices.data(); }
    int GetVertexNum() const {return vertices.size(); }
    int GetIndexNum() const {return indices.size(); }
    void SetVerticesArray(vector<QVector3D>& array){
        vertices = array;
    }
    void SetIndicesArray(vector<QVector3D>& array)
    {
        indices = array;
    }

private:
    int index_num_;
    int vertex_num_;
    vector<QVector3D> vertices;
    vector<int> indices;
};

#endif // TRIMESH_H
