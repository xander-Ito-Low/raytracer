/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the entry point function for the program you need to create for lab two.
 * You should not need to modify this code.
 * It creates a framebuffer, loads an triangle mesh object, calls the drawing function to render the object and then outputs the framebuffer as a ppm file.
 *
 * On linux.bath.ac.uk:
 *
 * Compile the code using g++ -o lab2executable main.cpp directional_light.cpp framebuffer.cpp phong.cpp polymesh.cpp sphere.cpp camera.cpp scene.cpp  pinholeCamera.cpp triangle.cpp pointLight.cpp BoundingBox.cpp thinLens.cpp -std=c++11

 

 pinholeCamera.cpp
 *
 * Execute the code using ./lab2executable
 *
 * This will produce an image file called test.ppm. You can convert this a png file for viewing using
 *
 * pbmropng test.ppm > test.png
 *
 * You are expected to fill in the missing code in polymesh.cpp.
 */

#include "framebuffer.h"
#include "ray.h"
//#include "hit.h"
#include "polymesh.h"
#include "sphere.h"
#include "light.h"
#include "directional_light.h"
#include "pointLight.h"
#include "material.h"
#include "phong.h"
#include "pinholeCamera.h"
#include "thinLens.h"
#include "scene.h"
#include "triangle.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;


//Generate Cornell test scene for the final image.
void createCornellScene(Scene *scene,ThinLens *cam,FrameBuffer *fb,int width, int height){
      
    // The following transform allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
    
    Transform *transform = new Transform(0.5f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.5f, 0.0f, -2.0f,
                                         0.0f, 0.0f, 0.5f, 7.0f,
                                         0.0f,0.0f,0.0f,1.0f);
        
//      Read in the teapot model.
      PolyMesh *pm = new PolyMesh((char *)"teapot.ply", transform);

    Phong bp20;
           bp20.ambient.r = 0.4f;
           bp20.ambient.g = 0.0f;
           bp20.ambient.b = 0.0f;
           bp20.diffuse.r = 0.4f;
           bp20.diffuse.g = 0.0f;
           bp20.diffuse.b = 0.0f;
           bp20.specular.r = 0.2f;
           bp20.specular.g = 0.2f;
           bp20.specular.b = 0.2f;
           bp20.power = 40.0f;
        
            pm->material = &bp20;
            pm->material->type = 0;

    Vertex v0;
              v0.x = 2.0f;
              v0.y = 0.0f;
              v0.z = 10.0f;

    Sphere *sphere = new Sphere(v0,0.75f);
    
    
    Phong bp21;
    bp21.ambient.r = 0.0f;
    bp21.ambient.g = 0.0f;
    bp21.ambient.b = 0.0f;
    bp21.diffuse.r = 0.0f;
    bp21.diffuse.g = 0.0f;
    bp21.diffuse.b = 0.0f;
    bp21.specular.r = 0.0f;
    bp21.specular.g = 0.0f;
    bp21.specular.b = 0.0f;
    bp21.power = 40.0f;
    sphere->material = &bp21;

    sphere->material->ior = 1.53;
    sphere->material->type = 3;
    
    Vertex v30;
    v30.x = 2.0f;
    v30.y = 2.0f;
    v30.z = 10.0f;

    Sphere *sphere2 = new Sphere(v30,0.75f);
    
    Phong bp22;
    bp22.ambient.r = 0.5;
    bp22.ambient.g = 0.06;
    bp22.ambient.b = 0.0f;
    bp22.diffuse.r = 0.5f;
    bp22.diffuse.g = 0.4f;
    bp22.diffuse.b = 0.0f;
    bp22.specular.r = 0.3f;
    bp22.specular.g = 0.3f;
    bp22.specular.b = 0.3f;
    bp22.power = 40.0f;
    sphere2->material = &bp22;
    sphere2->material->type = 1;
    sphere2->material->kr = 0.3;
    
    Vertex v31;
    v31.x = -3.0f;
    v31.y = -3.0f;
    v31.z = 7.0f;
    
    Sphere *sphere3 = new Sphere(v31,0.75f);
    
    Phong bp23;
    bp23.ambient.r = 0.0f;
    bp23.ambient.g = 0.0f;
    bp23.ambient.b = 0.0f;
    bp23.diffuse.r = 0.0f;
    bp23.diffuse.g = 0.0f;
    bp23.diffuse.b = 0.0f;
    bp23.specular.r = 0.0f;
    bp23.specular.g = 0.0f;
    bp23.specular.b = 0.0f;
    bp23.power = 40.0f;
    sphere3->material = &bp21;
    sphere3->material->type = 3;

    Vertex v32;
    v32.x = -1.0f;
    v32.y = 2.5f;
    v32.z = 10.0f;
    
    Sphere *sphere4 = new Sphere(v32,0.75f);
    
    Phong bp24;
    bp24.ambient.r = 0.4f;
    bp24.ambient.g = 0.0f;
    bp24.ambient.b = 0.4f;
    bp24.diffuse.r = 0.3f;
    bp24.diffuse.g = 0.0f;
    bp24.diffuse.b = 0.3f;
    bp24.specular.r = 0.2f;
    bp24.specular.g = 0.2f;
    bp24.specular.b = 0.2f;
    bp24.power = 40.0f;
    sphere4->material = &bp24;
    sphere4->material->type = 0;
    
    Ray ray;
    ray.position = cam->eye;

    Vertex v;
      v.x = -5.0f;
      v.y = -5.0f;
      v.z = 15.0f;

       Vertex v2;
      v2.x = 5.0f;
      v2.y = -5.0f;
      v2.z = 15.0f;

      Vertex v3;
      v3.x = -5.0f;
      v3.y = 5.0f;
      v3.z = 15.0f;

      Vertex v4;
      v4.x = 5.0f;
      v4.y = 5.0f;
      v4.z = 15.0f;

      Vertex v5;
      v5.x = -5.0f;
      v5.y = -5.0f;
      v5.z = 5.0f;

      Vertex v6;
      v6.x = -5.0f;
      v6.y = 5.0f;
      v6.z = 5.0f;

      Vertex v7;
      v7.x = 5.0f;
      v7.y = 5.0f;
      v7.z = 5.0f;

      Vertex v8;
      v8.x = 5.0f;
      v8.y = -5.0f;
      v8.z = 5.0f;

      Triangle *t1 = new Triangle(v4,v2,v);
      
    Triangle *t2 = new Triangle(v3,v4,v);
      
    Triangle *t3 = new Triangle(v,v5,v3);
      Triangle *t4 = new Triangle(v5,v6,v3);
      Triangle *t5 = new Triangle(v4,v7,v2);
      Triangle *t6 = new Triangle(v2,v7,v8);
      
    Triangle *t7 = new Triangle(v3,v4,v7);
    Triangle *t8 = new Triangle(v3,v7,v6);
      
    Triangle *t9 = new Triangle(v2,v5,v);
    Triangle *t10 = new Triangle(v2,v8,v5);

//    DirectionalLight *dl = new DirectionalLight(Vector(1.01f, 1.0f, 1.0f),Colour(1.0f, 1.0f, 1.0f, 0.0f));
    PointLight *pl = new PointLight(Colour(1.0f, 1.0f, 1.0f, 0.0f),Vertex(0,2.5,10.0));
//    pl->next = dl;

    Phong bp0;
        bp0.ambient.r = 0.0f;
        bp0.ambient.g = 0.0f;
        bp0.ambient.b = 0.0f;
        bp0.diffuse.r = 0.25f;
        bp0.diffuse.g = 0.25f;
        bp0.diffuse.b = 0.25f;
        bp0.specular.r = 0.0f;
        bp0.specular.g = 0.0f;
        bp0.specular.b = 0.0f;
        bp0.power = 40.0f;

    Phong bp1;
    bp1.ambient.r = 0.0f;
     bp1.ambient.g = 0.0f;
     bp1.ambient.b = 0.0f;
     bp1.diffuse.r = 0.37f;
     bp1.diffuse.g = 0.37f;
     bp1.diffuse.b = 0.37f;
     bp1.specular.r = 0.0f;
     bp1.specular.g = 0.0f;
     bp1.specular.b = 0.0f;
     bp1.power = 40.0f;

    Phong bp2;

    bp2.ambient.r = 0.0f;
      bp2.ambient.g = 0.0f;
      bp2.ambient.b = 0.0f;
      bp2.diffuse.r = 0.0f;
      bp2.diffuse.g = 0.5f;
      bp2.diffuse.b = 0.0f;
      bp2.specular.r = 0.0f;
      bp2.specular.g = 0.0f;
      bp2.specular.b = 0.0f;
      bp2.power = 40.0f;

      Phong bp3;

      bp3.ambient.r = 0.0f;
        bp3.ambient.g = 0.0f;
        bp3.ambient.b = 0.0f;
        bp3.diffuse.r = 0.0f;
        bp3.diffuse.g = 0.0f;
        bp3.diffuse.b = 0.5f;
        bp3.specular.r = 0.0f;
        bp3.specular.g = 0.0f;
        bp3.specular.b = 0.0f;
        bp3.power = 40.0f;

    t1->material = &bp0;
    t1->material->type = 0;
    t1->next = t2;
    t2->material = &bp0;
    t2->material->type = 0;
    t2->next = t3;
    t3->material = &bp2;
    t3->material->type = 0;
    t3->next = t4;
    t4->material = &bp2;
    t4->material->type = 0;
    t4->next = t5;
    t5->material = &bp3;
    t5->material->type = 0;
    t5->next = t6;
    t6->material = &bp3;
    t6->material->type = 0;
    t6->next = t7;
    t7->material = &bp1;
    t7->material->type = 0;
    t7->next = t8;
    t8->material = &bp1;
    t8->material->type = 0;
    t8->next = t9;
    t9->material = &bp1;
    t9->material->type = 0;
    t9->next = t10;
    t10->material = &bp1;
    t10->material->type = 0;
    t10->next = pm;
    pm->next = sphere;
    sphere->next = sphere2;
    sphere2->next = sphere3;
    sphere3->next = sphere4;

    scene->object_list = t1;
    scene->light_list =  pl;
    scene->firstPass(5,scene->object_list,scene->light_list);
    cam->renderPhoton(fb,scene,1,5,512,512);
}

//Debugging scene
void createThreeSphereScene(Scene *scene,PinholeCamera *cam,FrameBuffer *fb,int width, int height){
    Ray ray;
          ray.position = cam->eye;
        
        Vertex v;
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 3.0f;
        
         Vertex v2;
          v2.x = 1.5f;
          v2.y = 0.0f;
          v2.z = 6.0f;
                
        Vertex v3;
         v3.x = 3.0f;
         v3.y = 0.0f;
         v3.z = 5.0f;
        
        Sphere *sphere = new Sphere(v,0.75f);
        
        Sphere *sphere2 = new Sphere(v2,0.75f);

        Sphere *sphere3 = new Sphere(v3,0.75f);
        
        DirectionalLight *dl = new DirectionalLight(Vector(1.01f, -1.0f, 1.0f),Colour(1.0f, 1.0f, 1.0f, 0.0f));
        
          Phong bp1;
           bp1.ambient.r = 0.0f;
           bp1.ambient.g = 0.0f;
           bp1.ambient.b = 0.0f;
           bp1.diffuse.r = 0.0f;
           bp1.diffuse.g = 0.0f;
           bp1.diffuse.b = 0.0f;
           bp1.specular.r = 0.0f;
           bp1.specular.g = 0.0f;
           bp1.specular.b = 0.0f;
           bp1.power = 40.0f;
        
          Phong bp2;
        
          bp2.ambient.r = 0.0f;
            bp2.ambient.g = 0.2f;
            bp2.ambient.b = 0.0f;
            bp2.diffuse.r = 0.0f;
            bp2.diffuse.g = 0.2f;
            bp2.diffuse.b = 0.0f;
            bp2.specular.r = 0.6f;
            bp2.specular.g = 0.6f;
            bp2.specular.b = 0.6f;
            bp2.power = 40.0f;
        
            Phong bp3;
        
            bp3.ambient.r = 0.0f;
              bp3.ambient.g = 0.0f;
              bp3.ambient.b = 0.2f;
              bp3.diffuse.r = 0.0f;
              bp3.diffuse.g = 0.0f;
              bp3.diffuse.b = 0.2f;
              bp3.specular.r = 0.6f;
              bp3.specular.g = 0.6f;
              bp3.specular.b = 0.6f;
              bp3.power = 40.0f;
        
            sphere->material = &bp1;
           sphere->material->ior = 1.53;
            sphere->material->kr = 0.2;
            sphere->material->kt = 0.8;
            sphere->material->type = 3;
            sphere->next = sphere2;

             sphere2->material = &bp2;
            sphere2->material->kr = 0.8f;
            sphere2->material->type = 1;
             sphere2->next = sphere3;

            sphere3->material = &bp3;
            sphere3->material->kr = 0.6f;
            sphere3->material->type = 1;
        
        
           scene->object_list = sphere;
           scene->light_list =  dl;
           cam->renderScene(fb,scene,1,1,width,height);
}

int main(int argc, char *argv[])
{
    
  //initiate seed for all random calls
  srand(time(NULL));
  int width = 512;
  int height = 512;
  // Create a framebuffer
  FrameBuffer *fb = new FrameBuffer(width,height);
  Scene *scene = new Scene();

  // The following transform allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
//  Transform *transform = new Transform(1.0f, 0.0f, 0.0f,  0.0f,
//				       0.0f, 0.0f, 1.0f, -2.7f,
//				       0.0f, 1.0f, 0.0f, 5.0f,
//				       0.0f, 0.0f, 0.0f, 1.0f);
    
//  Read in the teapot model.
//  PolyMesh *pm = new PolyMesh((char *)"teapot_smaller.ply", transform);
 
 //Set up camera

//Uncomment if you want to use the pinhole camera, you also have to change the argument in the test function to accomodate
//PinholeCamera *cam = new PinholeCamera(0.5);
    
//Uncomment if you want to use the thin lens camera
float dist = 1.0, focus = 10.0, radius = 0.2;
ThinLens *cam = new ThinLens(dist,focus,radius);

//createThreeSphereScene(scene,cam,fb,width,height);
createCornellScene(scene,cam,fb,width,height);
  
}
