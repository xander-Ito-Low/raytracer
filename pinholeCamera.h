#pragma once
#include "camera.h"
#include "framebuffer.h"
#include "poissonDisk.h"
#include "scene.h"

using namespace std;

class PinholeCamera: public Camera {
    public:
    float d;
    
    PinholeCamera(float d);
    PinholeCamera(Vertex eye,Vertex lookAt,Vector Up,float d);
    
    Vector ray_direction(float x,float y);
   
    void renderScene(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height);
    
    void renderPhoton(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height);
};

