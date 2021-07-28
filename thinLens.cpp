#include "thinLens.h"
#include <iostream>
#include <math.h>
using namespace std;

/*Default constructor*/
ThinLens::ThinLens(float dist,float focus,float radius){
     Camera();
     d  = dist;
     f = focus;
     lensRadius = radius;
     
}

/*Constructor that takes in arbitrary values*/
ThinLens::ThinLens(Vertex eye,Vertex lookAt,Vector Up,float dist,float focus,float radius){
    Camera(eye,lookAt,Up);
    d = dist;
    f = focus;
    lensRadius = radius;
}

/*Sample points on the lens positioned at the eye*/
void ThinLens::generatePointsInCircle(int sampleSize){
    for(int i = 0; i<sampleSize; i++){
        for(int j = 0; j<sampleSize; j++){
       float randomVal1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        
        float randomVal2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        
        float theta = 2*M_PI*randomVal1;
        float r = (float)sqrt(randomVal2);
        float randomX =  r * lensRadius * cos(theta);
        float randomY =  r * lensRadius * sin(theta);
        Vector genPoint = Vector(randomX,randomY,eye.z);
        points.push_back(genPoint);
        }
    }
}

Vector ThinLens::ray_direction(float pixelPointX,float pixelPointY,float lensPointX,float lensPointY){
    
    //calculare the direction of the viewing ray using equations from Suffern 2016
    float focalPointX = pixelPointX * (f/d);
    float focalPointY = pixelPointY * (f/d);
    
    Vector result;
    result.x = (focalPointX-lensPointX)*u.x + (focalPointY-lensPointY)*(v.x) - (f*w.x);
    
    result.y = (focalPointX-lensPointX)*u.y + (focalPointY-lensPointY)*(v.y) - (f*w.y);
    
    result.z = (focalPointX-lensPointX)*u.z + (focalPointY-lensPointY)*(v.z) - (f*w.z);
    
    result.normalise();
    return result;
}

/*Render the scene using this camera*/
void ThinLens::renderScene(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height){
    
    Ray ray;
    if(sampleSize>1){
        generatePointsInCircle(sampleSize);
    }
    
    else{
        //default sample points in lens
        generatePointsInCircle(8);
    }
    
    
    for (int y = 0; y < height; y += 1){
        for (int x = 0; x < width; x += 1){
            Colour colour;
            /*Anti-aliasing*/
            if(sampleSize>1){
                poissonSampling sample;
                sample.generatePoints(x,y,sampleSize,width,height);
                /*loop through points on the lens and trace rays into the scene*/
                for(int i = 0; i<sample.points.size(); i++){
                    //random pixel points
                    float px = sample.points[i].x;
                    float py = sample.points[i].y;
                    
                    //random points on lens
                    float lensX = points[i].x;
                    float lensY = points[i].y;
                    
                    ray.position.x = eye.x + (lensX*u.x) + (lensY*v.x);
                    ray.position.y = eye.y + (lensX*u.y) + (lensY*v.y);
                    ray.position.z = eye.z + (lensX*u.z) + (lensY*v.z);
                    ray.direction = ray_direction(px,py,lensX,lensY);
                    
                    Colour contrColour = scene->raytrace(ray,depth,scene->object_list, scene->light_list);
                          colour.add(contrColour);
                }
                       //Average the colour value
                       float scale = (float)1/(float)sampleSize*sampleSize;
                       colour = colour.reduce(scale);
            }
            
            //otherwise, sample 1 center for multiple points on a lens. Default to 8
            else{
                for(int i = 0; i<64; i++){
                    float fx = (-0.5+(float(x)/float(width)));
                    float fy = ((float(y)/float(height)));
                    fy = 0.5f-fy;
                                   
                    float lensX = points[i].x;
                    float lensY = points[i].y;
                    
                    ray.position.x = eye.x + (lensX*u.x) + (lensY*v.x);
                    ray.position.y = eye.y + (lensX*u.y) + (lensY*v.y);
                    ray.position.z = eye.z + (lensX*u.z) + (lensY*v.z);
                    ray.direction = ray_direction(fx,fy,lensX,lensY);
                    Colour contrColour = scene->raytrace(ray,depth,scene->object_list, scene->light_list);
                    colour.add(contrColour);
                }
                float scale = (float)1/(float)64;
                colour = colour.reduce(scale);
            }
                    
            fb->plotPixel(x, y, colour.r, colour.g, colour.b);
        }
         cerr << "*" << flush;
    }
              
        // Output the framebuffer.
        fb->writeRGBFile((char *)"test.ppm");
}

/*Render the scene using this camera*/
void ThinLens::renderPhoton(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height){
    
    Ray ray;
    if(sampleSize>1){
        generatePointsInCircle(sampleSize);
    }
    
    else{
        //default sample points in lens
        generatePointsInCircle(8);
    }
    
    
    for (int y = 0; y < height; y += 1){
        for (int x = 0; x < width; x += 1){
            Colour colour;
            /*Anti-aliasing*/
            if(sampleSize>1){
                poissonSampling sample;
                sample.generatePoints(x,y,sampleSize,width,height);
                /*loop through points on the lens and trace rays into the scene*/
                for(int i = 0; i<sample.points.size(); i++){
                    //random pixel points
                    float px = sample.points[i].x;
                    float py = sample.points[i].y;
                    
                    //random points on lens
                    float lensX = points[i].x;
                    float lensY = points[i].y;
                    
                    ray.position.x = eye.x + (lensX*u.x) + (lensY*v.x);
                    ray.position.y = eye.y + (lensX*u.y) + (lensY*v.y);
                    ray.position.z = eye.z + (lensX*u.z) + (lensY*v.z);
                    ray.direction = ray_direction(px,py,lensX,lensY);
                    
                    Colour contrColour = scene->secondPass(ray,depth,scene->object_list, scene->light_list);
                          colour.add(contrColour);
                }
                       //Average the colour value
                       float scale = (float)1/(float)sampleSize*sampleSize;
                       colour = colour.reduce(scale);
            }
            
            //otherwise, sample 1 center for multiple points on a lens. Default to 8
            else{
                for(int i = 0; i<64; i++){
                    float fx = (-0.5+(float(x)/float(width)));
                    float fy = ((float(y)/float(height)));
                    fy = 0.5f-fy;
                                   
                    float lensX = points[i].x;
                    float lensY = points[i].y;
                    
                    ray.position.x = eye.x + (lensX*u.x) + (lensY*v.x);
                    ray.position.y = eye.y + (lensX*u.y) + (lensY*v.y);
                    ray.position.z = eye.z + (lensX*u.z) + (lensY*v.z);
                    ray.direction = ray_direction(fx,fy,lensX,lensY);
                    Colour contrColour = scene->secondPass(ray,depth,scene->object_list, scene->light_list);
                    colour.add(contrColour);
                }
                float scale = (float)1/(float)64;
                colour = colour.reduce(scale);
            }
                    
            fb->plotPixel(x, y, colour.r, colour.g, colour.b);
        }
         cerr << "*" << flush;
    }
              
        // Output the framebuffer.
        fb->writeRGBFile((char *)"test.ppm");
}

