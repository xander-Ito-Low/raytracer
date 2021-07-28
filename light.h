/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Light is the base class for lights.

#pragma once

#include "vertex.h"
#include "vector.h"
#include "colour.h"
#include "ray.h"

class Light {
public:
	Light *next;
    char source;

	Light()
	{
		next = (Light *)0;
	}

	// Get the direction towards the light at the point on the surface
	// should return true if the surface is in front of the light
	// and false if it's behind and not illuminated.
	virtual bool get_direction(Vertex &surface, Vector &dir)
	{
		return false;
	}

	// Get the intensity of the light in the direction of the surface
	virtual void get_intensity(Vertex &surface, Colour &intensity)
	{

    }
    
    virtual void get_intensityAtSource(Colour &level){

    }
    
    virtual void EmitPhotons(Ray *arr,int numberOfPhotons){
        
    }

};
