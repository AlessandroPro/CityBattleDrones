//
//  Polygon.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-01.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include "Vector2D.hpp"
#include "Vector3D.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#include <iostream>
#else
#include <GL/glut.h>
#endif
#include <vector>

using namespace std;

class Polygon
{
    public:
        
        Vector3D normal;
        std::vector<Vector3D> verts;
        bool isFrontFacing;
    
        Polygon();
        void calculateNormal();
        void draw();
        void draw(int texID, vector<Vector2D> stCoordinates, bool hasAlpha);
};

#endif /* Polygon_hpp */
