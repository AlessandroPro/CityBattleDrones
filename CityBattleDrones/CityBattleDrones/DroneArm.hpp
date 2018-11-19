//
//  DroneArm.hpp
//  Assignment1
//
//

#ifndef DroneArm_h
#define DroneArm_h

#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"
#include "Propeller.hpp"
#include "PrismMesh.hpp"
#include <vector>

// Material properties
static GLfloat arm_mat_ambient[] = { 0.4F, 0.2F, 0.05F, 1.0F };
static GLfloat arm_mat_specular[] = { 0.3F, 0.3F, 0.1F, 1.0F };
static GLfloat arm_mat_diffuse[] = { 0.1F, 0.1F, 0.8F, 0.0F };
static GLfloat arm_mat_shininess[] = { 0.8F };

using namespace std;

// A standard drone arm, comes with a propeller and propeller base
class DroneArm
{
private:
    
    GLfloat armLength;
    GLfloat armWidth;
    GLfloat propBaseScaleY;
    GLfloat propBaseRadius;
    int numPropBlades;
    Propeller propeller;
    PrismMesh cube;
    
    
public:
    
    DroneArm();
    DroneArm(GLfloat armLength, GLfloat armWidth, GLfloat propBaseScaleY, int numPropBlades);
    void draw();
    void spinPropeller();
};

#endif /* DroneArm_h */
