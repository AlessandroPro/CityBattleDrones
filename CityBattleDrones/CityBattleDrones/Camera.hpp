//
//  Camera.hpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-09.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "Vector3D.hpp"

#define PI 3.14159265
#define DEGTORAD (PI / 180.0)
#define DEFAULT_ZOOM 0.5

#include <iostream>

using namespace std;


class Camera
{
    public:
    
    float azimuth;
    float elevation;
    float zoomDistance;
    float azimuthChangeRate;
    float elevationChangeRate;
    float zoomChangeRate;
    float clickX;
    float clickY;
    bool clickAndDrag;
    float minElevation = 5;
    float maxElevation = 85;
    float minZoomDistance = 0.01;
    float maxZoomDistance = 20;
    Vector3D position;
    Vector3D focus;
    Vector3D forward;
    //boolean array to keep track of when a control is actioned:
    //changing azimuth, changing elevation, zooming in and out
    bool controlActions[3];
    
    Camera();
    void update();
    void setAzimuthChangeRate(float rate);
    void setElevationChangeRate(float rate);
    void setZoomChangeRate(float rate);
    void setElevation(float angle);
    void setAzimuth(float angle);
    void setZoom(float distance);
    void changeFocus(Vector3D newFocus);
    void move(float mouseX, float mouseY);
    void setMinElevation(float newMin);
    void setMaxElevation(float newMax);
};

#endif /* Camera_hpp */
