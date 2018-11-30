//
//  DroneAI.cpp
//  CityBattleDrones
//
//  Created by Alessandro Profenna on 2018-11-29.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#include "DroneAI.hpp"

DroneAI::DroneAI():
    Drone(),
    lastDecisionTime(0)
    {}

DroneAI::DroneAI(GLfloat scaleFactor, int numArms, int numPropBlades, Vector3D& spawnPoint, int maxNumMissiles):
    Drone(scaleFactor, numArms, numPropBlades, spawnPoint, maxNumMissiles),
    lastDecisionTime(0)
    {}

void DroneAI::lookAt(Vector3D pos)
{
    Vector3D lookAt = Vector3D::subtract(pos, position);
    Vector2D lookAtXZ = Vector2D(lookAt.z, lookAt.x);
    lookAtXZ.normalize();
    
    forward.x = lookAtXZ.y;
    forward.z = lookAtXZ.x;
    rotation.y = RADTODEG * atan(lookAtXZ.y/lookAtXZ.x);
}

void DroneAI::moveToward(Vector3D pos)
{
    float distance = Vector3D::subtract(pos, position).getLength();
    float chance = (rand() % 100)/100.0;
    
    if(chance < 0.6)
    {
        if(distance < 0.5)
        {
            setAction(3, true);
            setAction(2, false);
        }
        else if(distance < 4)
        {
            setAction(3, false);
            setAction(2, false);
        }
        else
        {
            setAction(3, false);
            setAction(2, true);
        }
    }
    
    if(pos.y > position.y + scaleFactor*5 && chance < 0.3)
    {
        cout << distance << "\n";
        setAction(0, true);
        setAction(1, false);
    }
    else if(pos.y < position.y - scaleFactor*5 && chance < 0.3)
    {
        setAction(1, true);
        setAction(0, false);
    }
    else
    {
        setAction(0, false);
        setAction(1, false);
    }
    
    if(chance < 0.3)
    {
        int random = rand() % 4;
        if(random == 0)
        {
            setAction(7, true);
            setAction(6, false);
        }
        else if(random == 1)
        {
            setAction(6, true);
            setAction(7, false);
        }
        else
        {
            setAction(7, false);
            setAction(6, false);
        }
    }
    
}


void DroneAI::decideToLaunch(Vector3D pos)
{
    float distance = Vector3D::subtract(pos, position).getLength();
    float chance = (rand() % 100)/100.0;
    
    if(distance < 1)
    {
        launchMissile();
    }
    // The farther the enemy drone is from the player, the less
    // likely it is to shoot;
    else if(chance < 1/distance)
    {
        launchMissile();
    }
}

void DroneAI::makeDecisions(Vector3D pos)
{
    lookAt(pos);
    
    int timeSinceLastDecision = glutGet(GLUT_ELAPSED_TIME) - lastDecisionTime;
    if(timeSinceLastDecision > 300)
    {
        moveToward(pos);
        decideToLaunch(pos);
        lastDecisionTime = glutGet(GLUT_ELAPSED_TIME);
    }
}

