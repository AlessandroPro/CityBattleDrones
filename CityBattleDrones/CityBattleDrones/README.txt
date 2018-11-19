README

Alessandro Profenna
CPS511 - Assignment 1
Drone Construction and Manipulation

This program was developed on a Mac using Xcode in C++.
There are two ways to compile and run this program on a Mac:

1) Open a new C++ Xcode project and place all of the files in the project folder. Run it.
2) There is a makefile for this project. To compile with it, cd into the project directory and
run 'make' -> this will compile all of the necessary files and generate an 'assignment1' executable. 
Run './assignment1' to run the program and control-c to terminate it.

NOTE: There are deprecation compiler warnings because of GLUT, but these can be ignored.
NOTE: Although there is code in the makefile for compiling on Windows, it has not been tested.
Therefore this assignment should be compiled, run, and marked on a Mac. If the TA does not have access
to a Mac, I can showcase the assignment running on my Mac to the TA at a designated time.

_____________________________________________


Project Structure:

The new files written by me are Drone.cpp, DroneArm.hpp, and Propeller.hpp, along with their respective header files. The program runs from main.cpp, which I modified in multiple places to add controls, change materials, initialize the drone, etc.

The Vector3D and QuadMesh files were untouched but are used in the main program.


_____________________________________________


Fulfilled Requirements:

All 4 major requirement sections are complete.
The 2 bonus questions were not attempted.

The comments explaining the CTM construction and vertex matrix multiplication can be found in all files where Push/PopMatrix is called. Each transformation matrix is labeled in the order that they appear. R1, R2, R3, etc. for rotations, T1, T2, T3, etc. for translations, and S1, S2, S3, etc. for scalings. Identity matrix is labeled with the letter I.

For example, construction of the propeller blades:

        // CTM = I * T1 * R1 * R2 * S1 * R3 * T6
        glPushMatrix();
        // Rotates the blade to its designated spot around the pivot's up axis
        // with the spin offset applied
        glRotatef(spinAngle, 0.0, 1.0, 0.0); //R4
        glTranslatef(bladeLength/2, 0.0, 0.0); //T7
        glScalef(bladeLength/2, 0.25*pivotRadius, pivotRadius); //S8
        // p' = I * T1 * R1 * R2 * S1 * R3 * T6 * R4 * T7 * S8 * p
        glutSolidSphere(1.0, 20.0, 20.0); // propeller blade
        glPopMatrix();


Here are the controls for the drone:

Move Up:                   Up Arrow Key
Move Down:                 Down Arrow Key
Rotate ClockWise:          Right Arrow Key
Rotate CounterClockWise:   Left Arrow Key
Move Forward:              f Key
Move Backward:             b Key
Start Spinning Propellers: s Key
Stop Spinning Propellers:  s Key
Print Controls:            h Key

NOTE: These controls are printed to stdout when the 'h' key is pressed while the program is running.


Also, the project folder includes screenshots of what the program should look like when it is running.


