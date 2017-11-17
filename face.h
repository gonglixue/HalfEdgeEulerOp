#ifndef FACE_H
#define FACE_H

#include "solid.h"
#include "loop.h"
class Loop;
class Solid;
class Face;

class Face
{
public:
    Face();
    bool AddLoop(Loop* new_loop);

    int face_id_;
    Solid *solid_; // the solid this face belongs to? neccessary?

    Face* prev_face_;
    Face* next_face_;

    //Loop* outer_loop_;
    //Loop* inner_loop_;
    Loop* loop_;
};

#endif // FACE_H
