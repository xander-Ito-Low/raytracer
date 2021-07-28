/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Material is the base class for materials.

#pragma once

#include "vector.h"
#include "colour.h"

class Material {
public:
    //Refractive index of the object
    float ior;
    float kt;
    float kr;
    int type;
    
	virtual void compute_base_colour(Colour &result)
	{
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}
	virtual void compute_light_colour(Vector &viewer, Vector &normal, Vector &ldir, Colour &result)
	{
		result.r = 0.0f;
		result.g = 0.0f;
		result.b = 0.0f;
	}
    
    virtual void compute_photonDiffuse(Vector &normal, Vector &ldir,Colour &inColour ,Colour &result){
        result.r = 0.0;
        result.g = 0.0;
        result.b = 0.0;
    }
    
    virtual void compute_photonCaustic(Vector &normal, Vector &ldir,Colour &inColour ,Colour &result){
           result.r = 0.0;
           result.g = 0.0;
           result.b = 0.0;
       }
    
    virtual void get_diffuse(Colour &result){
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;
    }
    
    virtual void get_specular(Colour &result){
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;
    }
    
    virtual void get_ambient(Colour &result){
          result.r = 0.0f;
          result.g = 0.0f;
          result.b = 0.0f;
      }
};
