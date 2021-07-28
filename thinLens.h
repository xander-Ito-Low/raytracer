#pragma once
#include "camera.h"
#include "framebuffer.h"
#include "poissonDisk.h"
#include "scene.h"
#include <vector>

using namespace std;

class ThinLens: public Camera {
    public:
    float lensRadius;
    float d;
    float f;
    //the points included in the disk
    vector<Vector> points;
    
    //default constructor
    ThinLens(float dist,float focus,float radius);
    
   //Customise everything
   ThinLens(Vertex eye,Vertex lookAt,Vector Up,float dist,float focus,float radius );
    
    //Generate random sample points on the lens
    void generatePointsInCircle(int sampleSize);
   
   
    Vector ray_direction(float pixelPointX,float pixelPointY,float lensPointX,float lensPointY);
   
    //render the scene according to requirements
    void renderScene(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height);
    
    //Render with photon mapping
    void renderPhoton(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height);
};

