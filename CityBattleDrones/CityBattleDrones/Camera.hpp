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
    const float minElevation = 5;
    const float maxElevation = 85;
    const float minZoomDistance = 3;
    const float maxZoomDistance = 20;
    Vector3D position;
    Vector3D focus;
    Vector3D forward;
    //boolean array to keep track of when a control is actioned:
    //changing azimuth, changing elevation, zooming in and out
    bool controlActions[3];
    
    Camera();
    void updatePosition();
    void setAzimuthChangeRate(float rate);
    void setElevationChangeRate(float rate);
    void setZoomChangeRate(float rate);
    void move(float mouseX, float mouseY);
};

#endif /* Camera_hpp */
