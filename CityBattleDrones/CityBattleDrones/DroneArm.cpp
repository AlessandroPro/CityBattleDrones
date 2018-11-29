//
//  DroneArm.cpp
//  Assignment1
//

#include "DroneArm.hpp"

DroneArm::DroneArm():
    armLength(1.0),
    armWidth(0.25),
    propBaseScaleY(2.0),
    propBaseRadius(armWidth*0.5),
    numPropBlades(2),
    propeller(Propeller(armLength/2, numPropBlades)),
    prism(PrismMesh(20)),
    destroyRotations(Vector3D(0,0,0))
{}
    
DroneArm::DroneArm(GLfloat armLength, GLfloat armWidth, GLfloat propBaseScaleY, int numPropBlades):
    armLength(armLength),
    armWidth(armWidth),
    propBaseScaleY(propBaseScaleY),
    propBaseRadius(armWidth*0.5),
    numPropBlades(numPropBlades),
    propeller(Propeller(armLength/2, numPropBlades)),
    prism(PrismMesh(20)),
    destroyRotations(Vector3D(0,0,0))
{}

// Draws the drone arm
void DroneArm::draw(){
    
    // Set drone arm material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, arm_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, arm_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, arm_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, arm_mat_shininess);
    
    vector<Vector2D> stCoordinates = {Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)};
    
    //draws arm
    glPushMatrix();
    glTranslatef(armLength/2, 0.0, 0.0);
    glScalef(armLength, armWidth, armWidth);
    glRotatef(90, 0, 0, 1);
    prism.draw(2001, stCoordinates, true, 2001);
    glPopMatrix();
        
    //draws propeller base
    glPushMatrix();
    glTranslatef(armLength, 0.0, 0.0);
    glScalef(propBaseRadius*2, propBaseRadius*2*propBaseScaleY, propBaseRadius*2);
    prism.draw(2001, stCoordinates, true, 2001);
    glPopMatrix();
    
    //Draws the propeller
    glPushMatrix();
    glTranslatef(armLength, propBaseRadius*propBaseScaleY, 0.0);
    propeller.draw();
    glPopMatrix();
}

// Offsets the rotation of each of the arm's propeller
void DroneArm::spinPropeller(){
    propeller.spin();
}

