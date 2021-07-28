
#pragma once

#include "vertex.h"
#include "object.h"

class BoundingBox : public Object {
    Vertex minCorner;
    Vertex maxCorner;
public:
    BoundingBox(Vertex min,Vertex max);
    bool intersection(Ray ray);
};
