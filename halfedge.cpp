#include "halfedge.h"

HalfEdge::HalfEdge()
{
    static int id = 0;
    half_edge_id_ = id++;

    next_he_ = prev_he_ = NULL;
    sym_he_ = NULL;
    loop_ = NULL;
    phy_e_ = NULL;
    start_v_ = NULL;
}
