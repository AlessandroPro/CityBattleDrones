//
//  Missile.hpp
//  CityBattleDrones
//
//

#ifndef Missile_hpp
#define Missile_hpp

#include <stdio.h>
#include "Vector3D.hpp"
#include "PrismMesh.hpp"
#include "Polygon.hpp"

#define PI 3.14159265
#define RADTODEG (180.0 / PI)

using namespace std;

class Missile
{
    float scaleFactor;
    float speed;
    float length;
    int bodyTexture;
    int headTexture;
    Vector3D position;
    Vector3D forward;
    Vector3D rotation;
    Vector3D targetPos;
    PrismMesh body;
    
public:
    
    bool isDestroyed;
    float timeDestroyed;
    
    Missile();
    Missile(float scaleFactor, float speed, Vector3D position, Vector3D forward, Vector3D targetPos, int bodyTex, int headTex);
    void moveForward(float speed);
    void alignRotation(Vector3D direction);
    void changeForward(Vector3D targetPos, float changeDelta);
    Vector3D getPosition();
    void setPosition(Vector3D newPos);
    void setForward(Vector3D newForward);
    void setTargetPos(Vector3D newTargetPos);
    void blowUp();
    void update();
    void draw();
};

#endif /* Missile_hpp */
