/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// DirectionaLight is a child class of Light and implements a light
// with constant value in a given direction. The light has no position
// and can be treated as infinitely far away.

#pragma once
#include "light.h"

class PointLight : public Light {
public:

	Vertex point;
    Vector direction;
	Colour intensity;
    int type;
    float power;

	PointLight();
    PointLight(Colour col,Vertex pt);
	PointLight(Vector dir,Colour col,Vertex pt,float n);
    
	void get_intensity(Vertex &surface, Colour &intensity);
    
    //get the intensity needed for photon mapping
    void get_intensityAtSource(Colour &level);
    
    void EmitPhotons(Ray *arr,int numberOfPhotons);
    float irand(int min, int max);
    
    bool get_direction(Vertex &surface, Vector &dir);

};
