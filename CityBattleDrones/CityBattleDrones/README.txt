README

ASSIGNMENT COMPLETED AS GROUP OF 2:
Alessandro Profenna 
Tolaz Hewa 

CPS511 - Assignment 3
City Battle Drones

This program was developed on a Mac using Xcode in C++.
There are two ways to compile and run this program on a Mac:

1) Open a new C++ Xcode project and place all of the files in the project folder. Run it.
2) There is a makefile for this project. To compile with it, cd into the project directory and
run 'make' -> this will compile all of the necessary files and generate an 'assignment3' executable. 
Run './assignment3' to run the program and control-c to terminate it.

NOTE: There are deprecation compiler warnings because of GLUT, but these can be ignored.
NOTE: Although there is code in the makefile for compiling on Windows, it has not been tested.
Therefore this assignment should be compiled, run, and marked on a Mac. If the TA does not have access to a Mac, we can showcase the assignment running on my Mac to the TA at a designated time.

_____________________________________________


Project Structure:

The newly written file in this project are:

--
main.cpp

Vector2D.cpp
Vector2D.hpp

Vector3D.cpp
Vector3D.hpp

Polygon.cpp 
Polygon.hpp

PrismMesh.cpp (Intead of using cubeMesh.cpp)
PrismMesh.hpp

Building.cpp
Building.hpp

Camera.cpp
Camera.hpp

Drone.cpp
Drone.hpp

DroneAI.cpp
DroneAI.hpp

DroneArm.cpp
DroneArm.hpp

Propeller.cpp
Propeller.hpp

Street.cpp
Street.hpp

Missile.cpp
Missile.hpp

Shaders:
vShader.glsl
fShader.glsl
--

Third-party files that are used but were not written by us:

--
Spline.cpp (used in constructing the building models)
Stb_image.cpp (used for loading texture images)
__


The program runs from main.cpp.



_____________________________________________


Fulfilled Requirements:

All required sections for teams of two are complete.

The bonus was also implemented.
Added special effects:
-puff of smoke when drones/missiles are destroyed
-skybox
-missiles have homing capabilities
-Map with real-time drone positions displayed

_____________________________________________


***How to use this program:***

The program is split into three viewports:

1) Third-Person Viewport: on the left, which is the view of the city through a third-person view of the drone player. The vertical angle of the camera can be changed by dragging up or down on the window.
2) First-Person Viewport: on the top right, which shows a view of the city through a first-person view of the drone player on a mounted under-side camera. 
3) City Map Viewport: on the bottom right, which shows a satellite view of the city as a map with real-time positional icons for the drone player and drone enemy.


Drones and Missiles:
There are two drones in this game, a Drone Player, controlled by the user, and a Drone Enemy, controlled by an AI. Both drones can shoot missiles but only up to 20 each can be in flight at a time, restocking the limit once they are destroyed.
The missiles have homing capabilities, so they will follow the drones if within a certain distance.
The Drone Enemy will actively chase and shoot missiles at the player.

Collisions:
The Drone Player can collide, and will explode, if it comes into contact with the Drone Enemy, the Drone Enemy's missiles, the ground, or any of the buildings, and will reappear at it's spawn point.
TheDrone Enemy can collide, and will explode, if it comes into contact with the Drone Player, the Drone Player's missiles, the ground, or any of the buildings, and will reappear at it's spawn point.
Since the Drone Enemy AI moves a lot, it may be difficult to test the drones colliding with each other, so to deactivate the Drone Enemy from moving, press the 'y' key. Press it again to reactivate it.
The missiles will collide/explode when coming into contact with the drone, world boundaries, buildings, or ground.


Buildings & Streets:
The buildings and streets were created using assignment2 and saved into CityMetaData3.txt.
They are loaded in and textured when the program begins.

Spotlight and mounted drone camera:
The spotlight is emitted from the drone in the first-person viewport and the mounted drone camera can be tilted.

Shaders:
The vertex shader is vShader.glsl and the fragment shader is fShader.glsl.
The spotlight is calculated in the fragment shader.

Controls:
The controls are printed to standard output when the 'h' key is pressed.


Here is the full list of controls:

-----------------------------------------------------
Move drone player forward:	w key
Move drone player backward:	s key
Move drone player left:		a key
Move drone player right:	d key

Rotate drone player left:	left arrow key
Rotate drone player right:	right arrow key
Move drone player up:		up arrow key
Move drone player down:		down arrow key
		
Change 3rd-person camera angle:	click and drag up/down on first-person view port
Zoom 3rd-person camera in:	n key
Zoom 3rd-person camera out:	m key

Activate/Deactivate Enemy AI:	y key

Launch missile:			spacebar

Tilt 1st-person camera down:	k key
Tilt 1st-person camera up:	i key

Print controls:			h key
Fullscreen:			f key
Quit the program:		ESC	
-----------------------------------------------------
	

Also, the project folder includes screenshots of what the program should look like when it is running.
