/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "triangle.h"
#include <math.h>


Triangle::Triangle(Vertex point1, Vertex point2, Vertex point3)
{
    
    p0 = point1;
    p1 = point2;
    p2 = point3;
}


// Moller-Trumbore
bool Triangle::rayTriangleIntersect(const Ray& ray, const Vector &v0, const Vector &v1, const Vector &v2, float &t)
{
  Vector p;
  Vector d;
  Vector e1,e2,h,s,q;
  float a,f,u,v;

  p.x = ray.position.x;
  p.y = ray.position.y;
  p.z = ray.position.z;
  d = ray.direction;

  e1 = v1 - v0;
  e2 = v2 - v0;

  d.cross(e2,h);
  a = e1.dot(h);

  if (a > -0.00001f && a < 0.00001f)
  {
    return false ;
  }

  f = 1/a;
  s = p - v0;
  u = f * s.dot(h);

  if (u < 0.0f || u > 1.0f)
  {
    return false;
  }

  s.cross(e1,q);
  v = f * d.dot(q);

  if ((v < 0.0f) || ((u + v) > 1.0f))
  {
    return false;
  }

  // compute t
 
  t = f * e2.dot(q);

  if (t > 0.00001f)
  {
    return true; // it's in front ray start
  }

  // it's behind you
  return false;
}

void Triangle::compute_face_normal(Vector &normal)
{
  Vector v0v1, v0v2;
    v0v1.x = p1.x - p0.x;
    v0v1.y = p1.y - p0.y;
    v0v1.z = p1.z - p0.z;
    
  v0v1.normalise();

  v0v2.x = p2.x - p0.x;
  v0v2.y = p2.y - p0.y;
  v0v2.z = p2.z - p0.z;

  v0v2.normalise();

  v0v1.cross(v0v2, normal);
  normal.normalise();
}

void Triangle::intersection(Ray ray, Hit &hit)
{
  hit.flag = false;
    
  Vector facenormal;
  compute_face_normal(facenormal);
    
  float ndotdir = facenormal.dot(ray.direction);

  if (fabs(ndotdir) < 0.000000001f)
  {
    // ray is parallel to triangle so does not intersect
    return;
  }

  Vector v0,v1,v2;
  v0.x = p0.x;
  v1.x = p1.x;
  v2.x = p2.x;

  v0.y = p0.y;
  v1.y = p1.y;
  v2.y = p2.y;

  v0.z = p0.z;
  v1.z = p1.z;
  v2.z = p2.z;


  Vector o;

  o.x = ray.position.x;
  o.y = ray.position.y;
  o.z = ray.position.z;
  float t,u,v;

  hit.flag =  rayTriangleIntersect(ray, v0, v1, v2, t) ;

    if (hit.flag == false){
        return;
        
    }
   
  if (t <= 0.0f)
  {
    // intersection is behind start of ray
    return;
  }

  Vertex p;

  p.x = ray.position.x + t * ray.direction.x;
  p.y = ray.position.y + t * ray.direction.y;
  p.z = ray.position.z + t * ray.direction.z;

  hit.t = t;
  hit.what = this;
  hit.position = p;

  hit.normal = facenormal;
  
//Double sided triangles
if(ray.direction.dot(hit.normal)>0){
      hit.normal.negate();
  }

  hit.normal.normalise();

  return;
}
