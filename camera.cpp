
//This class seems fine for now
#include "camera.h"
using namespace std;

 //default constructor, this will be used as a constructor to the pinhole camera
Camera::Camera(void){
    //some position behind the camera (remember behind is a +ve value)
    eye = Vertex(0.00001,0,0);
    lookAt = Vertex(0,0,0);
    //set up x and z-axis of the camera
    u = Vector(1,0,0);
    w = Vector (0,0,-1);
    //Y-axis
    v = Vector(0,1,0);
    up = Vector(0,1,0);
}

Camera::Camera(Vertex e,Vertex look,Vector Up){
    eye = e;
    lookAt = look;
    up = Up;
    compute_uvw();
}

void Camera::compute_uvw(){
    //this is to compute pointing out of screen vector
//    w = eye-lookAt;
    w.x = eye.x-lookAt.x;
    w.y = eye.y-lookAt.y;
    w.z = eye.z-lookAt.z;
    w.normalise();
//    u = up^w;
    up.cross(w,u);
    u.normalise();
//    v = w^u;
    w.cross(u,v);
}
