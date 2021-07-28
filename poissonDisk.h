#ifndef _POISSON_H_
#define _POISSON_H_

#include <cstdlib>
#include <math.h>
#include "vector.h"
#include <iostream>
#include <vector>

using namespace std;

//Calculate samples using jittered sampling
class poissonSampling{
public:
    
    vector<Vector> points; //the points included in poisson distrubution
    
    void generatePoints(int x, int y, int numberOfSamples,int width,int height){
        
        //loop through grid and perturb according to the random numbers generated
        for(int i = 0; i<numberOfSamples; i++){
               for(int j = 0; j<numberOfSamples; j++){
                   //choose a random value between 0 and 1
                   float randomValX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                   float randomValY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                   //Multiply this value by the frame
                   float fx = (-0.5+(float(x+randomValX)/float(width)));
                   float fy =((float(y+randomValY)/float(height)));
                   fy = 0.5f-fy;
                   Vector genPoint = Vector(fx,fy,0);
                   points.push_back(genPoint);
               }
           }
    }
                
};



#endif
