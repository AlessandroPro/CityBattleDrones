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
    spawnPoint(Vector3D(0.0, 0.0, 0.0)),
    rotation(Vector3D(0.0, 0.0, 0.0)),
    forward(Vector3D(0.0, 0.0, 1.0)),
    tiltAxis(Vector3D(0.0, 0.0, 0.0)),
    prism(PrismMesh(6)),
    controlActions{false, false, false, false, false, false, false, false},
    propsSpinning(false),
    isDestroyed(false),
    timeDestroyed(0)
{
    createArms(3.0, 0.5);
}

Drone::Drone(GLfloat scaleFactor, int numArms, int numPropBlades, Vector3D& spawnPoint):
    scaleFactor(scaleFactor),
    numArms(numArms),
    numPropBlades(numPropBlades),
    forwardSpeed(0.0),
    rightSpeed(0.0),
    tiltAngle(0.0),
    position(spawnPoint),
    spawnPoint(spawnPoint),
    rotation(Vector3D(0.0, 0.0, 0.0)),
    forward(Vector3D(0.0, 0.0, 1.0)),
    tiltAxis(Vector3D(0.0, 0.0, 0.0)),
    prism(PrismMesh(6)),
    controlActions{false, false, false, false, false, false},
    propsSpinning(false),
    isDestroyed(false),
    timeDestroyed(0)
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
    glPopMatrix();
}

// Draws the drone arms, evenly spaced around the drone's central up axis
void Drone::drawArms()
{
    for(int i = 0; i < arms.size(); i++){

        glPushMatrix();
        if(isDestroyed)
        {
            float speed = 1.5;
            float timeSinceDestroyed = glutGet(GLUT_ELAPSED_TIME) - timeDestroyed;
            float x = timeSinceDestroyed * scaleFactor * speed;
            float y = -0.1 * pow(x - scaleFactor*300, 2) + 0.1 * pow(scaleFactor*300, 2);
            // Rotates the arm to it's designated spot around the drone's central up axis
            glRotatef((360/numArms)*i * arms.at(i).destroyRotations.y, 0, 1, 0);
            glTranslatef(x, y, 0);
            glRotatef(timeSinceDestroyed * arms.at(i).destroyRotations.z/10, 0, 0, 1);
            glRotatef(timeSinceDestroyed * arms.at(i).destroyRotations.x/10, 1, 0, 0);
            if(timeSinceDestroyed > 2000)
            {
                respawn();
            }
        }
        else
        {
            // Rotates the arm to it's designated spot around the drone's central up axis
            glRotatef((360/numArms)*i, 0.0, 1.0, 0.0);
            spinPropellers();
        }
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
    
// Moves the drone in its forward direction by deltaForward, and
// in its right direction by deltaRight
// If deltaForward/deltaRIght are negative, the drone moves backward/left
// A combination of these two paramaters allows the drone to move in
// 8 possible directionsu
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

// Draws a cockpit
void Drone::drawCockpit()
{
    vector<Vector2D> stCoordinates = {Vector2D(0,0), Vector2D(0,1), Vector2D(1,1), Vector2D(1,0)};
    
    glPushMatrix();
    if(isDestroyed)
    {
        float timeSinceDestroyed = glutGet(GLUT_ELAPSED_TIME) - timeDestroyed;
        
        // Animates a smoke texture to represent the destruction of the drone
        glPushMatrix();
        vector<Vector3D> verts({Vector3D(1,-1,0), Vector3D(1,1,0), Vector3D(-1,1,0), Vector3D(-1,-1,0)});
        Polygon smoke;
        smoke.verts = verts;
        float animPercent = timeSinceDestroyed/1000;
        glColor4f(1.0, 1.0, 1.0, 1 - animPercent);
        glScalef(0.3 + animPercent*4, 0.3 + animPercent*5, 0.3 + animPercent*4);
        glScalef(3, 2, 3);
        smoke.draw(3000, stCoordinates, true);
        glPopMatrix();
        
        // Flings the destroyed cockpit
        float speed = 15;
        float x = timeSinceDestroyed * scaleFactor * speed;
        float y = -0.1 * pow(x - scaleFactor*3000, 2) + 0.1 * pow(scaleFactor*3000, 2);
        glTranslatef(x, y, 0);
    }
    glScalef(1.3, 1.2, 1.3);
    prism.draw(2001, stCoordinates, true, 2001);
    glTranslatef(0.0, 0.6, 0.0);
    glScalef(0.75, 0.75, 0.75);
    prism.draw(2001, stCoordinates, true, 2001);
    glTranslatef(0.0, 0.0, 0.5);
    glScalef(0.4, 0.4, 0.6);
    prism.draw(2001, stCoordinates, true, 2001);
    glPopMatrix();
}


void Drone::updateDrone()
{
    if(!isDestroyed)
    {
        forwardSpeed = 0;
        rightSpeed = 0;
        
        if(controlActions[0]) changeElevation(deltaMove);
        if(controlActions[1]) changeElevation(-deltaMove);
        
        forwardSpeed = ((controlActions[2]) ? deltaMove : forwardSpeed);
        forwardSpeed = ((controlActions[3]) ? -deltaMove : forwardSpeed);
        rightSpeed = ((controlActions[6]) ? deltaMove : rightSpeed);
        rightSpeed = ((controlActions[7]) ? -deltaMove : rightSpeed);
        
        if(controlActions[4]) changeDirection(2.0);
        if(controlActions[5]) changeDirection(-2.0);
        
        if(forwardSpeed == 0 && rightSpeed == 0) stabilize();
        else move(forwardSpeed, rightSpeed);
    }
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

void Drone::destroy()
{
    isDestroyed = true;
    timeDestroyed = glutGet(GLUT_ELAPSED_TIME);
    
    //Set random rotation scales for each arm for use in the destroy animation
    for(int i = 0; i < arms.size(); i++){
        float rotX = ((rand() % 60) - 30)/10.0; //between -3 and +3
        float rotY = ((rand() % 6) + 7)/10.0; //between 0.7 and 1.3
        float rotZ = ((rand() % 60) - 30)/10.0; //between -3 and +3
        arms.at(i).destroyRotations = Vector3D(rotX, rotY, rotZ);
    }
}

void Drone::respawn()
{
    isDestroyed = false;
    position = spawnPoint;
}


