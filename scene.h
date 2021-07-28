/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "colour.h"
#include "ray.h"
#include "object.h"
#include "light.h"
#include "hit.h"
#include "polymesh.h"
#include "sphere.h"
#include "material.h"
#include "phong.h"
#include "triangle.h"
#include <vector>


//KD tree library nanoflann
/*
 @misc{blanco2014nanoflann,
  title        = {nanoflann: a {C}++ header-only fork of {FLANN}, a library for Nearest Neighbor ({NN}) with KD-trees},
  author       = {Blanco, Jose Luis and Rai, Pranjal Kumar},
  howpublished = {\url{https://github.com/jlblancoc/nanoflann}},
  year         = {2014}
}
 */
#include "nanoflann.hpp"
using namespace nanoflann;

// Scene is a class that is used to build a scene database of objects
// and lights and then trace a ray through it.

class Scene {
public:
    //Follow the template design that Nanoflann recommends
    struct PointCloud{
        struct Point{
         float  x,y,z;
         Vector wi;
         Colour power;
         char type;
        };
        vector<Point> pts;
        
        //Return the size of the point cloud
        inline size_t kdtree_get_point_count() const {return pts.size();}
        
        //Get a specific point in the KD tree
        inline float kdtree_get_pt(const size_t idx, int dim) const
            {
                if (dim==0) return pts[idx].x;
                else if (dim==1) return pts[idx].y;
                else return pts[idx].z;
            }
        
        // Optional bounding-box computation: return false to default to a standard bbox computation loop.
            //   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
            //   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
            template <class BBOX>
            bool kdtree_get_bbox(BBOX& /*bb*/) const { return false; }
    
    };
    
    // construct a kd-tree index:
    typedef KDTreeSingleIndexAdaptor<
        L2_Simple_Adaptor<float, PointCloud> ,
        PointCloud,
        3 /* dim */
        > my_kd_tree_t;
    
    
    
    PointCloud globalPhotons;
    my_kd_tree_t globalPhotonMap;
    
    PointCloud causticPhotons;
    my_kd_tree_t causticPhotonMap;

	Object *object_list;
	Light *light_list;

	//Default constructor
    Scene();
    
	// Trace a ray through the scene and find the closest if any object
	// intersection in front of the ray.
    void trace(Ray ray, Object *objects, Hit &hit);
	
	// Trace a ray through the scene and return its colour. This function
	// is the one that should recurse down the reflection/refraction tree.
	Colour raytrace(Ray ray, int level, Object *objects, Light *lights);
    
    void firstPass(int level, Object *objects, Light *lights);
    Colour secondPass(Ray ray, int level, Object *objects, Light *lights);
    
    //Trace photons into the scene and see what objects they hit.
    //The resulting photons will be storred in a photon map
    void photonTrace(Ray ray,int level, Object *objects,Colour input,char type,int sort);
    int russianRoulette(float value1, float value2);
    
    void estimateIndirectDiffuse(Hit best_hit,Vertex hitPos,Colour &result);
    void estimateCaustic(Hit best_hit,Vertex hitPos,Colour &result);

};
