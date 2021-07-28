#ifndef _PHOTON_H_
#define _PHOTON_H_

#include "vertex.h"
#include "vector.h"
#include "colour.h"
#include <iostream>
using namespace std;

struct Photon{
    Vertex hitPosition;
    Vector IncidentDir;
    Colour intensity;
    char type;
};
    

    
#endif
