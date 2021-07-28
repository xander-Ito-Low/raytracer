#include "BoundingBox.h"
#include <math.h>

//Suffern 2016
BoundingBox::BoundingBox(Vertex min, Vertex max)
{
    minCorner = min;
    maxCorner = max;
}

bool BoundingBox::intersection(Ray ray)
    {
        //define the 2 hit points
        Vertex tmax;
        Vertex tmin;
        
        float a = 1.0/(float)ray.direction.x;
        if(a>=0){
            tmin.x = (minCorner.x - ray.position.x) * a;
            tmax.x = (maxCorner.x - ray.position.x) * a;
        }
        
        else {
            tmin.x = (maxCorner.x - ray.position.x) * a;
            tmax.x = (minCorner.x - ray.position.x) * a;
        }
        
        float b = 1.0 /(float)ray.direction.y;
        if (b >= 0) {
            tmin.y = (minCorner.y - ray.position.y) * b;
            tmax.y = (maxCorner.y - ray.position.y) * b;
        }
        
        else {
            tmin.y = (maxCorner.y - ray.position.y) * b;
            tmax.y = (minCorner.y - ray.position.y) * b;
        }

        float c = 1.0 /(float)ray.direction.z;
        if (c >= 0) {
            tmin.z = (minCorner.z - ray.position.z) * c;
            tmax.z = (maxCorner.z - ray.position.z) * c;
        }
        
        else {
            tmin.z = (maxCorner.z - ray.position.z) * c;
            tmax.z = (minCorner.z - ray.position.z) * c;
        }
        
        float t0, t1;

        // find largest entering t value

        if (tmin.x > tmin.y){
            t0 = tmin.x;
        }
        
        else{
            t0 = tmin.y;
        }

        if (tmin.z > t0){
            t0 = tmin.z;
        }

        // find smallest exiting t value

        if (tmax.x < tmax.y){
            t1 = tmax.x;
        }
        
        else{
            t1 = tmax.y;
        }

        if (tmax.z < t1){
            t1 = tmax.z;
        }

        return (t0 < t1 && t1 > 0.0001f);
    }


