//
//  Drone.hpp
//  Assignment1
//
//

#ifndef Drone_h
#define Drone_h

#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"
#include "DroneArm.hpp"
#include "PrismMesh.hpp"
#include <vector>
#include <math.h>
#include <iostream>

#define PI 3.14159265

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.05F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.3F, 0.3F, 0.1F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.1F, 0.1F, 0.8F, 0.0F };
static GLfloat drone_mat_shininess[] = { 0.8F };

using namespace std;

// A standard drone model, with a variable number of arms, base, and cockpit, and the ability to fly up, down, forward, backward, and rotate
// Also includes a variable number of arms, each with a spinnable propeller
class Drone
{
private:
    
    GLfloat scaleFactor;
    int numArms;
    int numPropBlades;
    float forwardSpeed;
    float rightSpeed;
    GLfloat tiltAngle;
    Vector3D position;
    Vector3D rotation;
    Vector3D forward;
    Vector3D tiltAxis;
    PrismMesh cube;
    vector<DroneArm> arms;
    //boolean array to keep track of when a control is actioned:
    //move up, move down, move forward, move backward,
    //rotate ccw, rotate cw, move right, move left
    bool controlActions[8];
    const GLfloat bodyScaleY = 0.65;
    
public:
    
    bool propsSpinning;
    
    Drone();
    Drone(GLfloat scaleFactor, int numArms, int numPropBlades, Vector3D& position);
    void draw();
    void drawArms();
    void createArms(GLfloat armLength, GLfloat armWidth);
    void changeElevation(GLfloat deltaY);
    void changeDirection(GLfloat deltaAngle);
    void spinPropellers();
    void move(GLfloat deltaForward, GLfloat deltaRight);
    void stabilize();
    void drawCockpit();
    void updateDrone();
    Vector3D getPosition();
    float getRotationY();
    void setAction(int actionIndex, bool set);
};

#endif /* Drone_h */
