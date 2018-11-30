//
//  DroneAI.hpp
//  CityBattleDrones
//
//  Created by Alessandro Profenna on 2018-11-29.
//  Copyright © 2018 Alessandro Profenna. All rights reserved.
//

#ifndef DroneAI_hpp
#define DroneAI_hpp

#include <stdio.h>
#include "Drone.hpp"
#include "Vector3D.hpp"
#include "Vector2D.hpp"

#define PI 3.14159265
#define RADTODEG (180.0 / PI)

using namespace std;

class DroneAI: public Drone
{
public:
    int lastDecisionTime;
    
    DroneAI();
    DroneAI(GLfloat scaleFactor, int numArms, int numPropBlades, Vector3D& spawnPoint, int maxNumMissiles);
    void lookAt(Vector3D pos);
    void moveToward(Vector3D pos);
    void decideToLaunch(Vector3D pos);
    void makeDecisions(Vector3D pos);
};

#endif /* DroneAI_hpp */
