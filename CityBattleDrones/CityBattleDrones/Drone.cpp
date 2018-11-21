//
//  Drone.cpp
//  Assignment1
//

#include "Drone.hpp"

Drone::Drone():
    scaleFactor(1.0),
    numArms(6),
    numPropBlades(2),
    forwardSpeed(0.0),
    rightSpeed(0),
    tiltAngle(0.0),
    position(Vector3D(0.0, 0.0, 0.0)),
    rotation(Vector3D(0.0, 0.0, 0.0)),
    forward(Vector3D(0.0, 0.0, 1.0)),
    tiltAxis(Vector3D(0.0, 0.0, 0.0)),
    cube(PrismMesh()),
    controlActions{false, false, false, false, false, false, false, false},
    propsSpinning(false)
{
    createArms(3.0, 0.5);
}

Drone::Drone(GLfloat scaleFactor, int numArms, int numPropBlades, Vector3D& position):
    scaleFactor(scaleFactor),
    numArms(numArms),
    numPropBlades(numPropBlades),
    forwardSpeed(0.0),
    rightSpeed(0.0),
    tiltAngle(0.0),
    position(position),
    rotation(Vector3D(0.0, 0.0, 0.0)),
    forward(Vector3D(0.0, 0.0, 1.0)),
    tiltAxis(Vector3D(0.0, 0.0, 0.0)),
    cube(PrismMesh()),
    controlActions{false, false, false, false, false, false},
    propsSpinning(false)
{
    createArms(3.0, 0.5);
}

void Drone::draw()
{
    // Set drone material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);
    

    glPushMatrix();
    // Translates drone to given position
    glTranslatef(position.x, position.y, position.z);
    
    // Rotates drone along the forward axis in its right/left direction
    if(controlActions[4]) glRotatef(tiltAngle, forward.x, forward.y, forward.z);
    else if(controlActions[5]) glRotatef(tiltAngle, -forward.x, -forward.y, -forward.z);
    
    // Rotates drone along the tilt axis in its forward/backward/left/right direction
    glRotatef(tiltAngle, tiltAxis.x, tiltAxis.y, tiltAxis.z);
    
    // Rotates the drone around the up axis to change its direction
    glRotatef(rotation.y, 0.0, 1.0, 0.0);
    
    // Scales the entire drone by scaleFactor
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    
    drawArms();
    
    // Set drone material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);
    
    drawCockpit();
    glScalef(1.0, bodyScaleY, 1.0);
    cube.draw();
    //glutSolidSphere(1.0, 20.0, 20.0); //drone body
    glPopMatrix();
}

// Draws the drone arms, evenly spaced around the drone's central up axis
void Drone::drawArms()
{
    for(int i = 0; i < arms.size(); i++){

        glPushMatrix();
        // Rotates the arm to it's designated spot around the drone's central up axis
        glRotatef((360/numArms)*i, 0.0, 1.0, 0.0);
        arms.at(i).draw();
        glPopMatrix();
    }
}

// Creates numArms arms for the drone, each sized with the given armLength and armWidth
void Drone::createArms(GLfloat armLength, GLfloat armWidth)
{
    for(int i = 0; i < numArms; i++){
        arms.push_back(DroneArm(armLength, armWidth, 2.0, numPropBlades));
    }
}

// Changes the elevation of the drone by deltaY
void Drone::changeElevation(GLfloat deltaY)
{
    position.y += deltaY;
    //To prevent the drone from beneath ground level
    if(position.y < -3.0){
        position.y = -3.0;
    }
}

// Changes the forward direction that the drone is facing by deltaAngle
void Drone::changeDirection(GLfloat deltaAngle)
{
    rotation.y += deltaAngle;
    forward.x = sin(rotation.y * PI / 180.0);
    forward.z = cos(rotation.y * PI / 180.0);
}

// Offsets the rotation of each of the drone arm's propellers
void Drone::spinPropellers()
{
    for(int i = 0; i < arms.size(); i++){
        arms.at(i).spinPropeller();
    }
}
    
// Moves the drone in its forward direction by deltaForward, which is the
// direction the cockpit is facing
// If deltaForward is negative, the drone moves backward
void Drone::move(GLfloat deltaForward, GLfloat deltaRight)
{
    Vector3D upDir(0.0, 1.0, 0.0);
    Vector3D right = Vector3D::crossProduct(forward, upDir);
    right.normalize();
    
    Vector3D velocityFB = Vector3D::scalarMul(forward, deltaForward);
    Vector3D velocityRL = Vector3D::scalarMul(right, deltaRight);
    
    Vector3D moveVelocity = Vector3D::add(velocityFB, velocityRL);
    tiltAxis = Vector3D::crossProduct(moveVelocity, upDir);
    position = Vector3D::add(position, moveVelocity);
    tiltAngle = -15.0;
}

// Resets the drone's tilt to make its body parallel with the ground;
// Called when the drone is idle
void Drone::stabilize()
{
    tiltAxis.x = 0.0;
    tiltAxis.y = 0.0;
    tiltAxis.z = 0.0;
    tiltAngle = 0.0;
}

// Draws a cockpit, which is made up of cube as well as sphere portruding
// the cube in the drone's forward direction
void Drone::drawCockpit()
{
    glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.75, 0.75, 0.75);
    cube.draw();
    //glutSolidCube(1.0); //cube cockpit
    glTranslatef(0.0, 0.0, 0.5);
    glScalef(0.4, 0.4, 0.6);
    cube.draw();
    //glutSolidSphere(1.0, 20.0, 20.0); //spherical cockpit window
    glPopMatrix();
}


void Drone::updateDrone()
{
    forwardSpeed = 0;
    rightSpeed = 0;
    
    if(controlActions[0]) changeElevation(0.5);
    if(controlActions[1]) changeElevation(-0.5);
    
    forwardSpeed = ((controlActions[2]) ? 0.5 : forwardSpeed);
    forwardSpeed = ((controlActions[3]) ? -0.5 : forwardSpeed);
    rightSpeed = ((controlActions[6]) ? 0.5 : rightSpeed);
    rightSpeed = ((controlActions[7]) ? -0.5 : rightSpeed);
    
    if(controlActions[4]) changeDirection(2.0);
    if(controlActions[5]) changeDirection(-2.0);
    
    if(forwardSpeed == 0 && rightSpeed == 0) stabilize();
    else move(forwardSpeed, rightSpeed);
}

Vector3D Drone::getPosition()
{
    return position;
}

float Drone::getRotationY()
{
    return rotation.y;
}

void Drone::setAction(int actionIndex, bool set)
{
    controlActions[actionIndex] = set;
}


