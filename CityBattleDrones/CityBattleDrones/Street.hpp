//
//  Street.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-30.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Street_hpp
#define Street_hpp

#include <stdio.h>
#include "Vector3D.hpp"
#include "Polygon.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Street
{
public:
    float scaleX;
    float scaleZ;
    float rotationY;
    Vector3D position;
    Polygon streetPoly;
    
    Street();
    void draw();
    void build();
    void rotateY(float deltaY);
    void moveAlongGround(float deltaX, float deltaZ);
    void changeScaleFactors(float deltaX, float deltaZ);
    string getMetaData();
    void processMetaData(string md);
    void draw(int textID);
    
};

#endif /* Street_hpp */
