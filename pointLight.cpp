/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "pointLight.h"
#include <time.h>

PointLight::PointLight()
{
	Light();
}

PointLight::PointLight(Colour col,Vertex pt)
{
	Light();

    //set up the point light
    point = pt;
    
   //Give it an intensity and a type depending on its constructor
	intensity = col;
    type = 0;
    source = 'P';
}

PointLight::PointLight(Vector dir,Colour col,Vertex pt,float n)
{
    Light();
    point = pt;
    intensity = col;
    direction = dir;
    direction.normalise();
    type = 1;
    power = n;
    source = 'P';
}

//Determines the direction of the pointlight. This will either be default direction or the direction of the pointlight
bool PointLight::get_direction(Vertex &surface, Vector &dir)
{
    
    Vector going;
    going.x = surface.x-point.x;
    going.y = surface.y-point.y;
    going.z = surface.z-point.z;
    going.normalise();
        
    if(!type){
        dir = going;
    }
    
    else{
        dir = direction;
    }

	return true;
}

void PointLight::get_intensityAtSource(Colour &level){
    level = intensity;
}

void PointLight::get_intensity(Vertex &surface, Colour &level)
{

    if(!type){
      level = intensity;
    }
    
    else{
        Vector posFromLight;
        posFromLight.x = surface.x-point.x;
        posFromLight.y = surface.y-point.y;
        posFromLight.z = surface.z-point.z;
        posFromLight.normalise();
        float n = direction.dot(posFromLight);
        float p = (float)pow(n, power);
        Colour ScaledCol = intensity.reduce(p);
        level = ScaledCol;
    }
}

//Photon emission for the point light
void PointLight::EmitPhotons(Ray *arr,int numberOfPhotons){
    
    int created = 0;
    while(created<numberOfPhotons){
        float x,y,z;
        do{
            float randx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float randy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float randz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            x = 2.0*randx-1.0;
            y = 2.0*randy-1.0;
            z = 2.0*randz-1.0;
        
        }while((x*x)+(y*y)+(z*z)>1);
        arr[created].position = point;
        
        arr[created].direction.x = x;
        arr[created].direction.y = y;
        arr[created].direction.z = z;
        arr[created].direction.normalise();
        
        created++;
    }
}
