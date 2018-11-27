//
//  Propeller.cpp
//  Assignment1
//

#include "Propeller.hpp"

Propeller::Propeller():
    bladeLength(0.3),
    pivotRadius(bladeLength*0.05),
    spinOff(0.0),
    numPropBlades(2)
{}
    
Propeller::Propeller(GLfloat bladeLength, int numPropBlades):
    bladeLength(bladeLength),
    pivotRadius(bladeLength*0.1),
    spinOff(0.0),
    numPropBlades(numPropBlades)
{}

// Draws the propeller
void Propeller::draw()
{
    //draws propeller pivot
    glPushMatrix();
    glScalef(pivotRadius, pivotRadius, pivotRadius);
    glutSolidSphere(1.0, 20.0, 20.0); // propeller pivot
    glPopMatrix();
    
    //draws all of the propeller blades
    drawBlades();
}

// Draws numPropBlades propeller blades and evenly spaces them around
// the pivot's central up axis;
// Also applies a rotational offset for each blade each redisplay that they're spinning
void Propeller::drawBlades()
{
    // Set propeller material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, prop_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, prop_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, prop_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, prop_mat_shininess);
    
    for(int i = 0; i < numPropBlades; i++){
        //draws propeller blade
        GLfloat spinAngle = (360.0/numPropBlades)*i + spinOff;
        if(spinAngle > 360.0){
            spinAngle -= 360.0;
        }
        glPushMatrix();
        // Rotates the blade to its designated spot around the pivot's up axis
        // with the spin offset applied
        glRotatef(spinAngle, 0.0, 1.0, 0.0);
        glTranslatef(bladeLength/2, 0.0, 0.0);
        glScalef(bladeLength/2, 0.25*pivotRadius, pivotRadius);
        glutSolidSphere(1.0, 20.0, 20.0); // propeller blade
        glPopMatrix();
    }
}

// Offsets the rotation of each propeller blade
void Propeller::spin()
{
    spinOff += 4.0;
    if(spinOff >360.0){
        spinOff -= 360.0;
    }
}



