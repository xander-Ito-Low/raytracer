/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>
#include "vertex.h"
#include <iostream>
using namespace std;

class Vector {
public:
	float x;
	float y;
	float z;

	Vector(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
	}

	Vector()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	void normalise()
	{
		float len = (float)sqrt((double)(x*x + y*y + z*z));
		x = x / len;
		y = y / len;
		z = z / len;
	}

	float len_sqr()
	{
	  return(x*x + y*y + z*z);
	}

	float length()
	{
	  return(float)sqrt((double)(x*x + y*y + z*z));
	}
    
    /*This is the distance between vectors*/
    float length(Vertex other)
    {
        return(float)sqrt((double)(x*other.x + y*other.y + z*other.z));
    }
	/*
	float dot(Vector &other)
	{
		return x*other.x + y*other.y + z*other.z;
	}
	*/
	float dot(Vector other)
	{
		return x*other.x + y*other.y + z*other.z;
	}


	void reflection(Vector initial, Vector &reflect)
	{
		float d;

		d = dot(initial);
		d = d*2.0f;

		reflect.x = initial.x - d * x;
		reflect.y = initial.y - d * y;
		reflect.z = initial.z - d * z;
	}
    
    int refraction(Vector &initial, Vector &refract,float ior)
    {
        float c1;
        //The normal vector
        Vector normal = Vector(x,y,z);
        
        //Index of refractions for both mediums
        float n1 = 1.0003, n2 = ior;
        c1 = dot(initial);

       //If the vectors point in opposite directions to one another, make the cos
       //positive
        if(c1<0){
            c1 = -c1;
        }
        
        //Otherwise we are on the inside of the medium swap n1 and n2
        else{
            swap(n1,n2);
            normal.negate();
        }
        
        float n = n1/n2;
        float nNegated = (float)1/n;
        float nNegatedSquare = nNegated*nNegated;
        float sinSquare = 1-(c1*c1);
    
        
        float c2Square = 1-(nNegatedSquare*sinSquare);
        
        if(c2Square<0){
            return 0;
        }
        
        else{
            float c2 = (float)sqrt(c2Square);
            
            refract.x = nNegated*initial.x - (c2-(nNegated*c1))*normal.x;
            refract.y = nNegated*initial.y - (c2-(nNegated*c1))*normal.y;
            refract.z = nNegated*initial.z - (c2-(nNegated*c1))*normal.z;
            return 1;
        }

    }
    
    //Compute kt and kr using the Frensnel equations
    float fresnel(Vector &initial,float ior)
    {
           //compute cosi
           float cosi = dot(initial);
           
           //Index of refractions for both mediums air will always be n1 unless changed
           float n1 = 1.0003, n2 = ior;
           if(cosi>0){
               swap(n1,n2);
           }
        
           float n = n1/n2;
           float nNegated = (float)1/n;
           float nNegatedSquare = nNegated*nNegated;
           float sinSquare = 1-(cosi*cosi);
           float c2Square = 1-(nNegatedSquare*sinSquare);
           
           //signifies total internal reflection
           if(c2Square<0){
               return 1;
           }
           
           else{
               //perform the frensel calculations
               float cost = (float)sqrt(c2Square);
               cosi = abs(cosi);
               
               float rpar = ((n*cosi)-cost)/((n*cosi)+cost);
               float rperp = (cosi-(n*cost))/(cosi+(n*cost));
               float kr = ((rpar*rpar)+(rperp*rperp))/(float)2;
               return kr;
               
           }
       }
    
   //Generate a random vector direction for hemisphere
    void generateRandomDirection(Vector &result){
        float x,y,z;
        do{
           float randx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
           float randy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
           float randz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
           x = 2.0*randx-1.0;
           y = 2.0*randy-1.0;
           z = 2.0*randz-1.0;
        }while((x*x)+(y*y)+(z*z)>1);
        
        result.x = x;
        result.y = y;
        result.z = z;
            
        //If the generated vector points in the other direction, negate the result
        if(dot(result)<0){
            result.negate();
        }
        
         result.normalise();
    }
    
    void swap (float &n1, float &n2){
        float temp = n1;
        n1 = n2;
        n2 = temp;
    }

	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void cross(Vector &other, Vector &result)
	{
	  result.x = y*other.z - z*other.y;
	  result.y = z*other.x - x*other.z;
	  result.z = x*other.y - y*other.x;
	}

	void cross(Vector &other)
	{
	  Vector result;
	  result.x = y*other.z - z*other.y;
	  result.y = z*other.x - x*other.z;
	  result.z = x*other.y - y*other.x;
	  x = result.x;
	  y = result.y;
	  z = result.z;
	}

	void add(Vector &other)
	{
	  x += other.x;
	  y += other.y;
	  z += other.z;
	}

	void sub(Vector &other)
	{
	  x -= other.x;
	  y -= other.y;
	  z -= other.z;
	}

    Vector& operator=(Vector other)
    {
      x = other.x;
      y = other.y;
      z = other.z;
      return *this;
    }

	friend Vector operator+(const Vector &a, const Vector &b)
	{
	  Vector t;
	  t.x = a.x + b.x;
	  t.y = a.y + b.y;
	  t.z = a.z + b.z;
	  return t;
	}
	friend Vector operator-(const Vector &a, const Vector &b)
	{
	  Vector t;
	  t.x = a.x - b.x;
	  t.y = a.y - b.y;
	  t.z = a.z - b.z;
	  return t;
	}

        friend Vector operator*(const Vector &a, const Vector &b)
		{
	  Vector t;
	  t.x = a.x * b.x;
	  t.y = a.y * b.y;
	  t.z = a.z * b.z;
	  return t;
	}

	        friend Vector operator*(const float a, const Vector &b)
		{
	  Vector t;
	  t.x = a * b.x;
	  t.y = a * b.y;
	  t.z = a * b.z;
	  return t;
	}

	
};

#endif
