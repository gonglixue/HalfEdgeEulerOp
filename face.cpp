#include "face.h"

Face::Face():prev_face_(NULL), next_face_(NULL), solid_(NULL), loop_(NULL)
{
    static int id = 0;
    face_id_ = id++;
}

bool Face::AddLoop(Loop* new_loop)
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
