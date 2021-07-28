#pragma once
#include "vertex.h"
#include "vector.h"
class Camera {
    
    public:
    /*Variables*/
    Vertex eye,lookAt;
   //The calculated vectors from the basis calculations
    Vector up,u,v,w;
    //This is the distance away from the eye
    float d;
    

    /*Constructor*/
    Camera();
    Camera(Vertex eye,Vertex lookAt,Vector Up);
//    Camera(Vertex e,Vertex look,Vector Up,float dist);
    
    /*Creation of a basis*/
    void compute_uvw();
    Vector ray_direction(float x,float y);
};

