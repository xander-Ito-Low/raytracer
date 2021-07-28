/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include "scene.h"
#include <iostream>
using namespace std;

Scene::Scene(): globalPhotonMap(3 /*dim*/,globalPhotons, KDTreeSingleIndexAdaptorParams(10 /* max leaf */)),causticPhotonMap(3 /*dim*/,causticPhotons, KDTreeSingleIndexAdaptorParams(10 /* max leaf */))
{
	object_list = 0;
	light_list = 0;
}


//This will create the photon map
void Scene::firstPass(int level, Object *objects, Light *lights){
    //For each light in the scene generate a random direciton vector
    Light *light = lights;
    while (light != (Light *)0){
        if(light->source =='P'){
            const int numOfPhotons = 50000;
            Ray rays [numOfPhotons];
            Colour intensity;
            light->get_intensityAtSource(intensity);
            light->EmitPhotons(rays,numOfPhotons);
            float divisor = (float)1/(float)(numOfPhotons);
            for(int i = 0; i<numOfPhotons; i++){
                Colour photonIntensity = intensity.reduce(divisor);
                photonTrace(rays[i],5,objects,intensity,'N',1);
            }
        
            //Caustic photon map calculation
            const int numberOfCaustics = 100000;
            while(causticPhotons.pts.size()<numberOfCaustics){
                Ray causticRays[numberOfCaustics];
                Colour intensity;
                light->get_intensityAtSource(intensity);
                light->EmitPhotons(rays,numOfPhotons);
                float divisor = (float)1/(float)(numberOfCaustics);
                for(int i = 0; i<numOfPhotons; i++){
                    Colour photonIntensity = intensity.reduce(divisor);
                    photonTrace(rays[i],5,objects,intensity,'N',2);
                }
            }
        }
        
          light = light->next;
    }
    cout<<"Photon map done";
    cout<<"\n";
     /* kd-tree construction */
    globalPhotonMap.buildIndex();
    causticPhotonMap.buildIndex();
}


//Just like ray tracing but for photons
void Scene::photonTrace(Ray ray,int level, Object *objects,Colour input,char type,int sort){
    Colour intensity = input;
    //check if we have not recursed too far
    level = level-1;
    if(level < 0){
        return;
    }

    //Find closest primitive to the lightsource
    Hit best_hit;
    trace(ray, objects, best_hit);

    if (best_hit.flag){
        
        //Case 1: Diffuse material with no specular reflection
        if(best_hit.what->material->type == 0){
            Colour photonDiffuse;
            
            if(type == 'S'){
                best_hit.what->material->compute_photonCaustic(best_hit.normal,ray.direction,input,photonDiffuse);
            }
            
            else{
                best_hit.what->material->compute_photonDiffuse(best_hit.normal,ray.direction,input,photonDiffuse);
            }
            
            //Store the photon in global photon map because the material is diffuse
             PointCloud::Point photon;
             photon.x = best_hit.position.x;
             photon.y = best_hit.position.y;
             photon.z = best_hit.position.z;
             photon.wi = ray.direction;
             photon.power = photonDiffuse;
             photon.type = type;
            
            //Decide to store in global photon map
            if(type=='I' && sort == 1){
                globalPhotons.pts.push_back(photon);
            }
            
            //Decide to store in caustic photon map
            else if(type =='S' && sort == 2){
                causticPhotons.pts.push_back(photon);
            }
            Colour diffuse;
            best_hit.what->material->get_diffuse(diffuse);
            
            //End of storage
            float kd = (diffuse.r + diffuse.g + diffuse.b)/(float)3;
            float absorb = 1-kd;
            int option = russianRoulette(kd,absorb);
            
            //Diffusely reflected
            if(option == 1){
                Ray diffuseReflectionRay;
                diffuseReflectionRay.position = best_hit.position;
                best_hit.normal.generateRandomDirection(diffuseReflectionRay.direction);
                
                //This will avoid reflection acne
                diffuseReflectionRay.direction.normalise();
                diffuseReflectionRay.position.x = diffuseReflectionRay.position.x + (0.0001f *diffuseReflectionRay.direction.x);
                diffuseReflectionRay.position.y = diffuseReflectionRay.position.y + (0.0001f *diffuseReflectionRay.direction.y);
                diffuseReflectionRay.position.z = diffuseReflectionRay.position.z + (0.0001f *diffuseReflectionRay.direction.z);
                //Indirectly reflected
                photonTrace(diffuseReflectionRay,level,objects,intensity,'I',sort);
            }
            
            //Photon is absorbed
            else if(option == 2){
                return;
            }
        }

        //Specular material
        if(best_hit.what->material->type == 1){
            //Get diffuse component and scale the incoming light by its BRDF
            Colour diffuse;
            best_hit.what->material->get_diffuse(diffuse);
            
            //get specular reflection coefficient
            Colour specular;
            best_hit.what->material->get_specular(specular);
            
            float kd = (diffuse.r + diffuse.g + diffuse.b)/(float)3;
            float ks = (specular.r + specular.g + specular.b)/(float)3;
            int option = russianRoulette(kd,ks);
            
            //Diffusely reflected
            if(option == 1){
                Ray diffuseReflectionRay;
                diffuseReflectionRay.position = best_hit.position;
                //Get random reflection direction
                best_hit.normal.generateRandomDirection(diffuseReflectionRay.direction);
                
                //This will avoid reflection acne
                diffuseReflectionRay.direction.normalise();
                diffuseReflectionRay.position.x = diffuseReflectionRay.position.x + (0.0001f *diffuseReflectionRay.direction.x);
                diffuseReflectionRay.position.y = diffuseReflectionRay.position.y + (0.0001f *diffuseReflectionRay.direction.y);
                diffuseReflectionRay.position.z = diffuseReflectionRay.position.z + (0.0001f *diffuseReflectionRay.direction.z);
                //Indirectly reflected
                photonTrace(diffuseReflectionRay,level,objects,intensity,'I',sort);
            }
            
            //Specular reflection
            else if(option == 2){
                Ray reflectionRay;
                reflectionRay.position = best_hit.position;
                best_hit.normal.reflection(ray.direction,reflectionRay.direction);
                               
                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);
                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);
                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);
                
                //Hit a diffuse surface before
                if(type == 'I'){
                     photonTrace(reflectionRay,level,objects,intensity,'I',sort);
                }
                
                //This ray has only been hitting specular surfaces
                else{
                    photonTrace(reflectionRay,level,objects,intensity,'S',sort);
                }
            }
            
            //Absorbed
            else{
                return;
            }
        }
        
        //Specular transmission material
        if(best_hit.what->material->type == 3){
            //Calculate the reflective coefficient based on direction of incomming light
            float kr = best_hit.normal.fresnel(ray.direction,best_hit.what->material->ior);
            float kt = 1-kr;
            if(kr!= 1){
                kt = kt-0.1;
            }
            
            int option =  russianRoulette(kr,kt);
            
            //reflected
            if(option == 1){
                Ray reflectionRay;
                reflectionRay.position = best_hit.position;
                best_hit.normal.reflection(ray.direction,reflectionRay.direction);
                               
                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);
                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);
                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);
                
                //Hit a diffuse surface before
                if(type == 'I'){
                     photonTrace(reflectionRay,level,objects,intensity,'I',sort);
                }
                
                //This ray has only been hitting specular surfaces
                else{
                    photonTrace(reflectionRay,level,objects,intensity,'S',sort);
                }
            }
            
            //transmitted
            if(option == 2){
                intensity.reduce(kt);
                Ray refractionRay;
                refractionRay.position = best_hit.position;
                best_hit.normal.refraction(ray.direction,refractionRay.direction,best_hit.what->material->ior);
                //This will avoid refraction acne
                refractionRay.direction.normalise();
                refractionRay.position.x = refractionRay.position.x + (0.0001f * refractionRay.direction.x);
                refractionRay.position.y = refractionRay.position.y + (0.0001f * refractionRay.direction.y);
                refractionRay.position.z = refractionRay.position.z + (0.0001f * refractionRay.direction.z);
                
                //Hit a diffues surface before
                if(type == 'I'){
                     photonTrace(refractionRay,level,objects,intensity,'I',sort);
                }
                
                //This ray has only been hitting specular surfaces
                else{
                    photonTrace(refractionRay,level,objects,intensity,'S',sort);
                }
            }
        }
    }
}

//2 instances of this: Reflected, Transmitted, Absorbed
// or: Diffuse reflection, specular reflection, Absorbed
int Scene::russianRoulette(float value1, float value2){
    float genVal = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if(genVal<value1){
        return 1;
    }

    else if(genVal<value1+value2){
        return 2;
    }

    else{
        return 3;
    }
}

void Scene::trace(Ray ray, Object *objects, Hit &hit)
{
    
    Hit current_hit;

    hit.flag = false;
    hit.t = 0.0f;
    hit.what = 0;

    while (objects != 0)
    {
        Hit hit_current;

        objects->intersection(ray, hit_current);

        if (hit_current.flag == true)
        {
            if (hit.flag == false)
            {
                hit = hit_current;

            } else if (hit_current.t < hit.t)
            {
                hit = hit_current;
            }
        }

        objects = objects->next;
    }
}

//Estimate for indirect diffuse reflections
void Scene::estimateIndirectDiffuse(Hit best_hit,Vertex hitPos,Colour &result){
    //Query the global photon map for the closest points within a given radius
    const float query_pt[3] = {hitPos.x,hitPos.y, hitPos.z};
    const float search_radius = 0.2;
    vector<pair<size_t, float>> ret_matches;
    SearchParams params;
    params.sorted = true;
    const size_t nMatches = globalPhotonMap.radiusSearch(&query_pt[0], search_radius, ret_matches, params);
   
    if(nMatches == 0){
        return;
    }

    for (size_t i = 0; i < nMatches; i++){
        PointCloud::Point photon = globalPhotons.pts[ret_matches[i].first];
        if(photon.type!= 'I'){
            continue;
        }

        else{
            Colour diffuse;
            Vector incomming = photon.wi;
            Colour photonColour  = photon.power;
            best_hit.what->material->compute_photonDiffuse(best_hit.normal,incomming,photonColour,diffuse);
             float filter = (float)1 -((float)2/(float)6);
            float divisor = (float)M_PI * (search_radius*search_radius)*filter;
            divisor = (float)1/divisor;
            diffuse.reduce(divisor);
            result.add(diffuse);
        }
    }
}


//Radiance estimates for caustic photons
void Scene::estimateCaustic(Hit best_hit,Vertex hitPos,Colour &result){
    //Query the global photon map for the closest points within a given radius
    const float query_pt[3] = {hitPos.x,hitPos.y, hitPos.z};
    const float search_radius = 0.05;
    vector<pair<size_t, float>> ret_matches;
    SearchParams params;
    params.sorted = true;
    const size_t nMatches = causticPhotonMap.radiusSearch(&query_pt[0], search_radius, ret_matches, params);
   
    if(nMatches == 0){
        return;
    }

    for (size_t i = 0; i < nMatches; i++){
        PointCloud::Point photon = causticPhotons.pts[ret_matches[i].first];
            Colour diffuse;
            Vector incomming = photon.wi;
            Colour photonColour  = photon.power;
            best_hit.what->material-> compute_photonCaustic(best_hit.normal,incomming,photonColour,diffuse);
            float filter = (float)1 -((float)2/(float)6);
            float divisor = (float)M_PI * (search_radius*search_radius) * filter;
            divisor = (float)1/divisor;
            diffuse.reduce(divisor);
            result.add(diffuse);
        }
}


//This will do the ray tracing with approximations of the photon map
Colour Scene::secondPass(Ray ray, int level, Object *objects, Light *lights){
    // a default colour if we hit nothing.
        Colour colour;

        float red   =  (float) 135/(float) 255;
        float green =  (float) 206/(float) 255;
        float blue  =  (float) 235/(float) 255;

        // check we've not recursed too far.
        level = level - 1;
        if (level < 0)
        {
            colour.r = red;
            colour.g = green;
            colour.b = blue;
            return colour;
        }

        // first step, find the closest primitive

        Hit best_hit;
        Hit shadow_hit;
        trace(ray, objects, best_hit);

        // if we found a primitive then compute the colour we should see
        if (best_hit.flag)
        {
               //Compute the inidrect irradiance here and other estimates from the photon map//
            estimateIndirectDiffuse(best_hit,best_hit.position,colour);
            estimateCaustic(best_hit,best_hit.position,colour);
            
            Colour ambient;
            best_hit.what->material->get_ambient(ambient);
            if(ambient.r+ambient.g+ambient.b>0.0){
                best_hit.what->material->compute_base_colour(colour);
                colour.reduce(0.01);
            }
            
               Light *light = lights;

                while (light != (Light *)0)
                {
                    Vector viewer;
                    Vector ldir;
                    light->get_direction(best_hit.position,ldir);

                    viewer.x = -best_hit.position.x;
                    viewer.y = -best_hit.position.y;
                    viewer.z = -best_hit.position.z;
                    viewer.normalise();

                     bool lit = true;
//                    if(lit &&best_hit.what->material->type!=3 && light->source == 'D')
//                    {
//                                       Ray shadow_ray;
//
//                                       shadow_ray.direction.x = ldir.x;
//                                       shadow_ray.direction.y = ldir.y;
//                                       shadow_ray.direction.z = ldir.z;
//                                       shadow_ray.position.x = best_hit.position.x + (0.0001f * shadow_ray.direction.x);
//                                       shadow_ray.position.y = best_hit.position.y + (0.0001f * shadow_ray.direction.y);
//                                       shadow_ray.position.z = best_hit.position.z + (0.0001f * shadow_ray.direction.z);
//
//                                       trace(shadow_ray, objects, shadow_hit);
//
//                                       if(shadow_hit.flag==true)
//                                       {
//                                           if (shadow_hit.t < 1000000000.0f)
//                                           {
//                                               lit = false; //there's a shadow so no lighting, if realistically close
//                                           }
//                                       }
//                                   }


                    if (lit)
                    {
                        Colour intensity;
                        Colour scaling;

                        light->get_intensity(best_hit.position, scaling);

                        best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);

                        intensity.scale(scaling);

                        colour.add(intensity);
                    }

                    light = light->next;
                }
            
            if(colour.r>1){
                colour.r = 1;
            }

            if(colour.g>1){
                colour.g = 1;
            }

            if(colour.b>1){
                colour.b = 1;
            }


            //compute reflection ray if material supports it.
            if(best_hit.what->material->type == 1)
            {
                //Create a reflection ray and recurse

                Ray reflectionRay;
                reflectionRay.position = best_hit.position;

                best_hit.normal.reflection(ray.direction,reflectionRay.direction);

                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

                float kr = best_hit.what->material->kr;
                Colour result = raytrace(reflectionRay,level,objects,lights).reduce(kr);
                colour.add(result);

                //If the returned colour is greater than the maximum brightness, set the colour to 1.
                if(colour.r>1){
                    colour.r = 1;
                }

                if(colour.g>1){
                    colour.g = 1;
                }

                if(colour.b>1){
                    colour.b = 1;
                }
            }

            //Compute refraction ray
            if(best_hit.what->material->type == 2)
            {
                Ray refractionRay;
                refractionRay.position = best_hit.position;

                //compute the direction of the refracted ray
                int value = best_hit.normal.refraction(ray.direction,refractionRay.direction,best_hit.what->material->ior);

                //total internal reflection
                if(value == 0){
                    Ray reflectionRay;
                    reflectionRay.position = best_hit.position;

                    best_hit.normal.reflection(ray.direction,reflectionRay.direction);

                    //This will avoid reflection acne
                    reflectionRay.direction.normalise();
                    reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

                    reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

                    reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

                    float kr = 1.0;
                    Colour result = raytrace(reflectionRay,level,objects,lights).reduce(kr);
                    colour.add(result);
                }

                else{

                //This will avoid refraction acne
                refractionRay.direction.normalise();

                refractionRay.position.x = refractionRay.position.x + (0.0001f * refractionRay.direction.x);
                refractionRay.position.y = refractionRay.position.y + (0.0001f * refractionRay.direction.y);
                refractionRay.position.z = refractionRay.position.z + (0.0001f * refractionRay.direction.z);

                float kt = best_hit.what->material->kt;
                Colour result1 = raytrace(refractionRay,level,objects,lights).reduce(kt);
                colour.add(result1);

                Ray reflectionRay;
                reflectionRay.position = best_hit.position;
                best_hit.normal.reflection(ray.direction,reflectionRay.direction);

                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

                float kr = best_hit.what->material->kr;
                Colour result2 = raytrace(reflectionRay,level,objects,lights).reduce(kr);
                colour.add(result2);

                }

                //If the returned colour is greater than the maximum brightness, set the colour to 1.
                if(colour.r>1){
                    colour.r = 1;
                }

                if(colour.g>1){
                    colour.g = 1;
                }

                if(colour.b>1){
                    colour.b = 1;
                }

            }

            //Frensnel equations
            if(best_hit.what->material->type == 3)
            {
                float kr = best_hit.normal.fresnel(ray.direction,best_hit.what->material->ior);
                //compute refraction
                if(kr < 1){
                    Ray refractionRay;
                    refractionRay.position = best_hit.position;
                    float kt = 1.0-kr;
                    //compute refraction direction
                    best_hit.normal.refraction(ray.direction,refractionRay.direction,best_hit.what->material->ior);
                    //This will avoid refraction acne
                    refractionRay.direction.normalise();

                    refractionRay.position.x = refractionRay.position.x + (0.0001f * refractionRay.direction.x);
                    refractionRay.position.y = refractionRay.position.y + (0.0001f * refractionRay.direction.y);
                    refractionRay.position.z = refractionRay.position.z + (0.0001f * refractionRay.direction.z);

                    Colour result1 = raytrace(refractionRay,level,objects,lights).reduce(kt);
                    colour.add(result1);
                }

                //compute reflection
                Ray reflectionRay;
                reflectionRay.position = best_hit.position;
                best_hit.normal.reflection(ray.direction,reflectionRay.direction);

                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);
                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

                Colour result2 = raytrace(reflectionRay,level,objects,lights).reduce(kr);
                colour.add(result2);

                if(colour.r>1){
                    colour.r = 1;
                }

                if(colour.g>1){
                    colour.g = 1;
                }

                if(colour.b>1){
                    colour.b = 1;
                }
            }
        }

        else{
            colour.r = red;
            colour.g = green;
            colour.b = blue;
        }

        return colour;
}


Colour Scene::raytrace(Ray ray, int level, Object *objects, Light *lights)
{

	// a default colour if we hit nothing.
    Colour colour;
//
//	float red   =  (float) 135/(float) 255;
//	float green =  (float) 206/(float) 255;
//	float blue  =  (float) 235/(float) 255;
    
    float red   = 0;
    float green =  0;
    float blue  = 0;

	// check we've not recursed too far.
	level = level - 1;
	if (level < 0)
	{
//        colour.r = 0;
//        colour.g = 0;
//        colour.b = 0;
        colour.r = red;
        colour.g = green;
        colour.b = blue;
        return colour;
	}

	// first step, find the closest primitive

	Hit shadow_hit;
    Hit best_hit;
	trace(ray, objects, best_hit);

	// if we found a primitive then compute the colour we should see
	if (best_hit.flag)
    {
            best_hit.what->material->compute_base_colour(colour);
            Light *light = lights;

            while (light != (Light *)0)
            {
                Vector viewer;
                Vector ldir;

                viewer.x = -best_hit.position.x;
                viewer.y = -best_hit.position.y;
                viewer.z = -best_hit.position.z;
                viewer.normalise();
            
                light->get_direction(best_hit.position,ldir);

                bool lit = true;

//                //This implementation can only deal with directional lights
//                if(lit &&best_hit.what->material->type!=3 && light->source == 'D')
//                {
//
//                    Ray shadow_ray;
//
//                    shadow_ray.direction.x = ldir.x;
//                    shadow_ray.direction.y = ldir.y;
//                    shadow_ray.direction.z = ldir.z;
//                    shadow_ray.position.x = best_hit.position.x + (0.0001f * shadow_ray.direction.x);
//                    shadow_ray.position.y = best_hit.position.y + (0.0001f * shadow_ray.direction.y);
//                    shadow_ray.position.z = best_hit.position.z + (0.0001f * shadow_ray.direction.z);
//
//                    trace(shadow_ray, objects, shadow_hit);
//
//                    if(shadow_hit.flag==true)
//                    {
//                        if (shadow_hit.t < 1000000000.0f)
//                        {
//                            lit = false; //there's a shadow so no lighting, if realistically close
//                        }
//                    }
//                }

                if (lit)
                {
                    Colour intensity;
                    Colour scaling;
                    light->get_intensity(best_hit.position, scaling);

                    best_hit.what->material->compute_light_colour(viewer, best_hit.normal, ldir, intensity);

                    intensity.scale(scaling);

                    colour.add(intensity);
                }

                light = light->next;
            }


		// TODO: compute reflection ray if material supports it.
        //
        if(best_hit.what->material->type == 1)
		{
            //Create a reflection ray and recurse

            Ray reflectionRay;
            reflectionRay.position = best_hit.position;

            best_hit.normal.reflection(ray.direction,reflectionRay.direction);

            //This will avoid reflection acne
            reflectionRay.direction.normalise();
            reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

            reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

            reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

            float kr = best_hit.what->material->kr;
            Colour result = raytrace(reflectionRay,level,objects,lights).reduce(kr);
            colour.add(result);

            //If the returned colour is greater than the maximum brightness, set the colour to 1.
            if(colour.r>1){
                colour.r = 1;
            }

            if(colour.g>1){
                colour.g = 1;
            }

            if(colour.b>1){
                colour.b = 1;
            }
		}

		// TODO: Compute refraction ray
		if(best_hit.what->material->type == 2)
        {
            Ray refractionRay;
            refractionRay.position = best_hit.position;

            //compute the direction of the refracted ray
            int value = best_hit.normal.refraction(ray.direction,refractionRay.direction,best_hit.what->material->ior);

            //total internal reflection
            if(value == 0){
                Ray reflectionRay;
                reflectionRay.position = best_hit.position;

                best_hit.normal.reflection(ray.direction,reflectionRay.direction);

                //This will avoid reflection acne
                reflectionRay.direction.normalise();
                reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

                reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

                reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

                float kr = 1.0;
                Colour result = raytrace(reflectionRay,level,objects,lights).reduce(kr);
                colour.add(result);
            }

            else{

            //This will avoid refraction acne
            refractionRay.direction.normalise();

            refractionRay.position.x = refractionRay.position.x + (0.0001f * refractionRay.direction.x);
            refractionRay.position.y = refractionRay.position.y + (0.0001f * refractionRay.direction.y);
            refractionRay.position.z = refractionRay.position.z + (0.0001f * refractionRay.direction.z);

            float kt = best_hit.what->material->kt;
            Colour result1 = raytrace(refractionRay,level,objects,lights).reduce(kt);
            colour.add(result1);

            Ray reflectionRay;
            reflectionRay.position = best_hit.position;
            best_hit.normal.reflection(ray.direction,reflectionRay.direction);

            //This will avoid reflection acne
            reflectionRay.direction.normalise();
            reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

            reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);

            reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

            float kr = best_hit.what->material->kr;
            Colour result2 = raytrace(reflectionRay,level,objects,lights).reduce(kr);
            colour.add(result2);

            }

            //If the returned colour is greater than the maximum brightness, set the colour to 1.
            if(colour.r>1){
                colour.r = 1;
            }

            if(colour.g>1){
                colour.g = 1;
            }

            if(colour.b>1){
                colour.b = 1;
            }

        }

        //Frensnel equations
        if(best_hit.what->material->type == 3)
        {
            float kr = best_hit.normal.fresnel(ray.direction,best_hit.what->material->ior);
            //compute refraction
            if(kr < 1){
                Ray refractionRay;
                refractionRay.position = best_hit.position;
                float kt = 1.0-kr;
                //compute refraction direction
                best_hit.normal.refraction(ray.direction,refractionRay.direction,best_hit.what->material->ior);
                //This will avoid refraction acne
                refractionRay.direction.normalise();

                refractionRay.position.x = refractionRay.position.x + (0.0001f * refractionRay.direction.x);
                refractionRay.position.y = refractionRay.position.y + (0.0001f * refractionRay.direction.y);
                refractionRay.position.z = refractionRay.position.z + (0.0001f * refractionRay.direction.z);

                Colour result1 = raytrace(refractionRay,level,objects,lights).reduce(kt);
                colour.add(result1);
            }

            //compute reflection
            Ray reflectionRay;
            reflectionRay.position = best_hit.position;
            best_hit.normal.reflection(ray.direction,reflectionRay.direction);

            //This will avoid reflection acne
            reflectionRay.direction.normalise();
            reflectionRay.position.x = reflectionRay.position.x + (0.0001f *reflectionRay.direction.x);

            reflectionRay.position.y = reflectionRay.position.y + (0.0001f *reflectionRay.direction.y);
            reflectionRay.position.z = reflectionRay.position.z + (0.0001f *reflectionRay.direction.z);

            Colour result2 = raytrace(reflectionRay,level,objects,lights).reduce(kr);
            colour.add(result2);

            if(colour.r>1){
                colour.r = 1;
            }

            if(colour.g>1){
                colour.g = 1;
            }

            if(colour.b>1){
                colour.b = 1;
            }
        }
    }

    else{
//        colour.r = 0.0;
//        colour.g = 0.0;
//        colour.b = 0.0;
        colour.r = red;
        colour.g = green;
        colour.b = blue;
    }

    return colour;
}
