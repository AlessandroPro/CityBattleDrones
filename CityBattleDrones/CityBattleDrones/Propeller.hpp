//
//  Propeller.hpp
//  Assignment1
//
//

#ifndef Propeller_h
#define Propeller_h

#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <stdlib.h>
#else
#include <GL/glut.h>
#endif
#include "Vector3D.hpp"

// Material properties
static GLfloat prop_mat_ambient[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat prop_mat_specular[] = { 0.65F, 0.6F, 0.6F, 1.0F };
static GLfloat prop_mat_diffuse[] = { 1.0F, 1.0F, 1.0F, 1.0F };
static GLfloat prop_mat_shininess[] = { 0.9F };

using namespace std;

// A standard propeller, with a pivot and blades, and the ability to spin
class Propeller
{
private:
    
    GLfloat bladeLength;
    GLfloat pivotRadius;
    GLfloat spinOff;
    int numPropBlades;
    
public:
    
    Propeller();
    Propeller(GLfloat bladeLength, int numPropBlades);
    void draw();
    void drawBlades();
    void spin();
};

#endif /* Propeller_h */
