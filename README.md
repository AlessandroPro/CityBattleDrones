# CityBattleDrones
An OpenGL C++ program that depicts an urban city with texture-mapped buildings and the ability to control a drone that can battle with a AI-controlled drone.

<img width="1440" alt="screenshot1" src="https://user-images.githubusercontent.com/15040875/50874427-fbf78880-1391-11e9-8206-c2646fe9dcf3.png">

This program was developed on a Mac using Xcode in C++.
There are two ways to compile and run this program on a Mac:

1) Open a new C++ Xcode project and place all of the files in the project folder. Run it.
2) There is a makefile for this project. To compile with it, cd into the project directory and
run 'make' -> this will compile all of the necessary files and generate an 'assignment3' executable. 
Run './assignment3' to run the program and control-c to terminate it.

NOTE: There are deprecation compiler warnings because of GLUT, but these can be ignored.
NOTE: Although there is code in the makefile for compiling on Windows, it has not been tested.

_____________________________________________

### **Project Structure:**

The newly written file in this project are:

--

main.cpp

Vector2D.cpp <br>
Vector2D.hpp

Vector3D.cpp <br>
Vector3D.hpp

Polygon.cpp <br>
Polygon.hpp

PrismMesh.cpp <br>
PrismMesh.hpp

Building.cpp <br>
Building.hpp

Camera.cpp <br>
Camera.hpp

Drone.cpp <br>
Drone.hpp

DroneAI.cpp <br>
DroneAI.hpp

DroneArm.cpp <br>
DroneArm.hpp

Propeller.cpp <br>
Propeller.hpp

Street.cpp <br>
Street.hpp

Missile.cpp <br>
Missile.hpp 

Shaders: <br>
vShader.glsl <br>
fShader.glsl

--

Third-party files that are used but were not written by us:

--

Spline.cpp (used in constructing the building models) <br>
Stb_image.cpp (used for loading texture images) <br>

__

The program runs from main.cpp.

_____________________________________________

**Added special effects:** <br>
-puff of smoke when drones/missiles are destroyed <br>
-skybox <br>
-missiles have homing capabilities <br>
-Map with real-time drone positions displayed <br>

_____________________________________________


### **How to use this program:**

The program is split into three viewports:

1) Third-Person Viewport: on the left, which is the view of the city through a third-person view of the drone player. The vertical angle of the camera can be changed by dragging up or down on the window.<br>
2) First-Person Viewport: on the top right, which shows a view of the city through a first-person view of the drone player on a mounted under-side camera. <br>
3) City Map Viewport: on the bottom right, which shows a satellite view of the city as a map with real-time positional icons for the drone player and drone enemy.


**Drones and Missiles:** <br>
There are two drones in this game, a Drone Player, controlled by the user, and a Drone Enemy, controlled by an AI. Both drones can shoot missiles but only up to 20 each can be in flight at a time, restocking the limit once they are destroyed. <br>
The missiles have homing capabilities, so they will follow the drones if within a certain distance. <br>
The Drone Enemy will actively chase and shoot missiles at the player.

**Collisions:** <br>
The Drone Player can collide, and will explode, if it comes into contact with the Drone Enemy, the Drone Enemy's missiles, the ground, or any of the buildings, and will reappear at it's spawn point. <br>
TheDrone Enemy can collide, and will explode, if it comes into contact with the Drone Player, the Drone Player's missiles, the ground, or any of the buildings, and will reappear at it's spawn point. <br>
Since the Drone Enemy AI moves a lot, it may be difficult to test the drones colliding with each other, so to deactivate the Drone Enemy from moving, press the 'y' key. Press it again to reactivate it. <br>
The missiles will collide/explode when coming into contact with the drone, world boundaries, buildings, or ground.

**Buildings & Streets:** <br>
The buildings and streets were created using assignment2 and saved into CityMetaData3.txt. <br>
They are loaded in and textured when the program begins. <br>

**Spotlight and mounted drone camera:** <br>
The spotlight is emitted from the drone in the first-person viewport and the mounted drone camera can be tilted.

**Shaders:** <br>
The vertex shader is vShader.glsl and the fragment shader is fShader.glsl.
The spotlight is calculated in the fragment shader.

**Controls:** <br>
The controls are printed to standard output when the 'h' key is pressed.


Here is the full list of controls:

-----------------------------------------------------
Action | Control
------------ | -------------
Move drone player forward |	w key 
Move drone player backward |	s key
Move drone player left	|	a key 
Move drone player right |	d key
<br>|<br>
Rotate drone player left |	left arrow key
Rotate drone player right |	right arrow key
Move drone player up |		up arrow key
Move drone player down |	down arrow key
<br>|<br>
Change 3rd-person camera angle |click and drag up/down on first-person view port
Zoom 3rd-person camera in |	n key
Zoom 3rd-person camera out |	m key
<br>|<br>
Activate/Deactivate Enemy AI |	y key
<br>|<br>
Launch missile |		spacebar
<br>|<br>
Tilt 1st-person camera down |	k key
Tilt 1st-person camera up |	i key
<br>|<br>
Print controls |		h key
Fullscreen |			f key
Quit the program |		ESC

-----------------------------------------------------
	

Additional Screenshots:

<img width="1415" alt="screenshot2" src="https://user-images.githubusercontent.com/15040875/50875789-ec7b3e00-1397-11e9-8b30-3b71a5c5af4f.png">

<img width="1413" alt="screenshot3" src="https://user-images.githubusercontent.com/15040875/50875794-f1d88880-1397-11e9-9df1-9dcea0cc3d25.png">

<img width="1410" alt="screenshot4" src="https://user-images.githubusercontent.com/15040875/50875795-f604a600-1397-11e9-9569-3b2effacf91a.png">

<img width="1411" alt="screenshot5" src="https://user-images.githubusercontent.com/15040875/50875801-fdc44a80-1397-11e9-8786-bd7c35e36f33.png">
