//
//  Camera.cpp
//  CityBuilder
//
//  Created by Alessandro Profenna on 2018-11-09.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "Camera.hpp"


Camera::Camera():
    azimuth(0),
    elevation(25),
    zoomDistance(0.001),
    clickX(0),
    clickY(0),
    clickAndDrag(false),
    azimuthChangeRate(3),
    elevationChangeRate(3),
    zoomChangeRate(3),
    position(Vector3D()),
    forward(Vector3D()),
    focus(Vector3D(0,0,0))
    {
        controlActions[0] = false;
        controlActions[1] = false;
        controlActions[2] = false;
        update();
    }

void Camera::update()
{
    if(controlActions[0]) azimuth += azimuthChangeRate;
    if(controlActions[1]) elevation += elevationChangeRate;
    if(controlActions[2]) zoomDistance -= zoomChangeRate;
    
    if(azimuth >= 360)
    {
        //azimuth -= 360;
    }
    else if(azimuth < 0)
    {
        //azimuth += 360;
    }
    
    if(elevation < minElevation)
    {
        elevation = minElevation;
    }
    else if(elevation > maxElevation)
    {
        elevation = maxElevation;
    }
    
    if(zoomDistance < minZoomDistance)
    {
        zoomDistance = minZoomDistance;
    }
    else if(zoomDistance > maxZoomDistance)
    {
        zoomDistance = maxZoomDistance;
    }
    
    //cout << azimuth << "\n";
    
    //change elevation
    position.x = 0;
    position.y = zoomDistance*sin(elevation*DEGTORAD);
    position.z = zoomDistance*cos(elevation*DEGTORAD);
    
    //change azimuth
    position.x = position.z*sin(azimuth*DEGTORAD);
    position.z = position.z*cos(azimuth*DEGTORAD);
    
    position = Vector3D::add(position, focus);
    
    forward = Vector3D::subtract(focus, position);
    forward.normalize();
}

void Camera::setAzimuthChangeRate(float rate)
{
    azimuthChangeRate = rate;
}

void Camera::setElevationChangeRate(float rate)
{
    elevationChangeRate = rate;
}

void Camera::setZoomChangeRate(float rate)
{
    zoomChangeRate = rate;
}

void Camera::setElevation(float angle)
{
    elevation = angle;
}

void Camera::setAzimuth(float angle)
{
    azimuth = angle;
}

void Camera::setZoom(float distance)
{
    zoomDistance = distance;
}

void Camera::changeFocus(Vector3D newFocus)
{
    focus = newFocus;
}

//Used for click and draggin on the screen window to move the camera around the hemisphere
void Camera::move(float mouseX, float mouseY)
{
    azimuth += (clickX-mouseX)/800;
    elevation += (mouseY-clickY)/8;
    clickX = mouseX;
    clickY = mouseY;
}
