/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "vertex.h"
#include "object.h"
#include "BoundingBox.h"

class Triangle : public Object {
    Vertex p0;
    Vertex p1;
    Vertex p2;
public:
	Triangle (Vertex point1, Vertex point2, Vertex point3);
    bool rayTriangleIntersect(const Ray& ray, const Vector &v0, const Vector &v1, const Vector &v2, float &t);
    void compute_face_normal(Vector &normal);
	void intersection(Ray ray, Hit &hit);
};
