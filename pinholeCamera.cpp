#include "pinholeCamera.h"
#include <iostream>
#include <math.h>
using namespace std;

/*Default constructor*/
PinholeCamera::PinholeCamera(float dist){
     Camera();
     d  = dist;
}

/*Constructor that takes in arbitrary values*/
PinholeCamera::PinholeCamera(Vertex eye,Vertex lookAt,Vector Up,float dist){
    Camera(eye,lookAt,Up);
    d = dist;
}

Vector PinholeCamera::ray_direction(float x,float y){
    //calculare the direction of the viewing ray
    Vector result;
    result.x = (x*u.x) + (y*v.x) - (d*w.x);
    result.y = (x*u.y) + (y*v.y) - (d*w.y);
    result.z = (x*u.z) + (y*v.z) - (d*w.z);
    result.normalise();
    return result;
}

void PinholeCamera::renderScene(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height){
    
    Ray ray;
    ray.position = eye;
    
    for (int y = 0; y < height; y += 1){
        for (int x = 0; x < width; x += 1){
            Colour colour;
            if(sampleSize>1){
                poissonSampling sample;
                sample.generatePoints(x,y,sampleSize,width,height);
                for(int i = 0; i<sample.points.size(); i++){
                    ray.direction = ray_direction(sample.points[i].x,sample.points[i].y);
                          Colour contrColour = scene->raytrace(ray,depth,scene->object_list, scene->light_list);
                          colour.add(contrColour);
                }
                       //Average the colour value
                       float scale = (float)1/(float)sampleSize*sampleSize;
                       colour = colour.reduce(scale);
            }
            
            //otherwise, sample 1 ray per pixel
            else{
                float fx = (-0.5+(float(x)/float(width)));
                float fy = ((float(y)/float(height)));
                fy = 0.5f-fy;
                ray.direction = ray_direction(fx,fy);
                colour = scene->raytrace(ray,depth,scene->object_list, scene->light_list);
            }
                    
            fb->plotPixel(x, y, colour.r, colour.g, colour.b);
        }
         cerr << "*" << flush;
    }
              
        // Output the framebuffer.
        fb->writeRGBFile((char *)"test.ppm");
}

void PinholeCamera::renderPhoton(FrameBuffer *fb,Scene *scene,int sampleSize,int depth,int width, int height){
    
    Ray ray;
    ray.position = eye;
    
    for (int y = 0; y < height; y += 1){
        for (int x = 0; x < width; x += 1){
            Colour colour;
            if(sampleSize>1){
                poissonSampling sample;
                sample.generatePoints(x,y,sampleSize,width,height);
                for(int i = 0; i<sample.points.size(); i++){
                    ray.direction = ray_direction(sample.points[i].x,sample.points[i].y);
                          Colour contrColour = scene->secondPass(ray,depth,scene->object_list, scene->light_list);
                          colour.add(contrColour);
                }
                       //Average the colour value
                       float scale = (float)1/(float)sampleSize*sampleSize;
                       colour = colour.reduce(scale);
            }
            
            //otherwise, sample 1 ray per pixel
            else{
                float fx = (-0.5+(float(x)/float(width)));
                float fy = ((float(y)/float(height)));
                fy = 0.5f-fy;
                ray.direction = ray_direction(fx,fy);
                colour = scene->secondPass(ray,depth,scene->object_list, scene->light_list);
            }
                    
            fb->plotPixel(x, y, colour.r, colour.g, colour.b);
        }
         cerr << "*" << flush;
    }
              
        // Output the framebuffer.
        fb->writeRGBFile((char *)"test.ppm");
}







































