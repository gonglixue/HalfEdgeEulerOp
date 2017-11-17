#include "edge.h"

Edge::Edge()
{
    static int id = 0;
    edge_id_ = id++;
    prev_edge_ = next_edge_ = NULL;
    he1_ = he2_ = NULL;
}
