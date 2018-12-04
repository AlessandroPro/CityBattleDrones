# Linux (default)
EXE = main
LDFLAGS = -lGL -lGLU -lglut

# Windows (cygwin)
ifeq ($(OS), "Windows_NT")
	EXE = hello.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif
# OS X
ifeq "$(shell uname)" "Darwin"
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

main : main.o Vector3D.o Vector2D.o Polygon.o PrismMesh.o Building.o Spline.o Camera.o Drone.o DroneArm.o Propeller.o DroneAI.o Street.o Stb_image.o Missile.o 
	g++ -std=c++11 -o assignment3 main.o Vector3D.o Vector2D.o Polygon.o PrismMesh.o Building.o Spline.o Camera.o Drone.o DroneArm.o Propeller.o DroneAI.o Street.o Stb_image.o Missile.o $(CFLAGS) $(LDFLAGS)

main.o : main.cpp Vector3D.hpp Polygon.hpp PrismMesh.hpp Building.hpp Camera.hpp Drone.hpp DroneAI.hpp Street.hpp Missile.hpp Stb_image.cpp
	g++ -std=c++11 -std=c++11 -c -w main.cpp 

Vector3D.o : Vector3D.cpp Vector3D.hpp
	g++ -std=c++11 -c -w Vector3D.cpp 

Vector2D.o : Vector2D.cpp Vector2D.hpp
	g++ -std=c++11 -c -w Vector2D.cpp 

Polygon.o : Polygon.cpp Polygon.hpp Vector3D.hpp Vector2D.hpp
	g++ -std=c++11 -c -w Polygon.cpp

Spline.o : Spline.cpp
	g++ -std=c++11 -c -w Spline.cpp

PrismMesh.o : PrismMesh.cpp PrismMesh.hpp Polygon.hpp Vector2D.hpp
	g++ -std=c++11 -c -w PrismMesh.cpp

Building.o : Building.cpp Building.hpp Polygon.hpp PrismMesh.hpp Spline.cpp Vector2D.hpp
	g++ -std=c++11 -c -w Building.cpp

Camera.o : Camera.cpp Camera.hpp Vector3D.hpp
	g++ -std=c++11 -c -w Camera.cpp

Drone.o : Drone.cpp Drone.hpp DroneArm.hpp Vector3D.hpp PrismMesh.hpp Missile.hpp
	g++ -std=c++11 -c -w Drone.cpp

DroneArm.o : DroneArm.cpp DroneArm.hpp Propeller.hpp PrismMesh.hpp Vector2D.hpp Vector3D.hpp
	g++ -std=c++11 -c -w DroneArm.cpp

Propeller.o : Propeller.cpp Propeller.hpp Vector3D.hpp
	g++ -std=c++11 -c -w Propeller.cpp

DroneAI.o : DroneAI.cpp DroneAI.hpp Drone.hpp Vector2D.hpp Vector3D.hpp
	g++ -std=c++11 -c -w DroneAI.cpp

Street.o : Street.cpp Street.hpp Polygon.hpp Vector3D.hpp
	g++ -std=c++11 -c -w Street.cpp

Stb_image.o : Stb_image.cpp
	g++ -std=c++11 -c -w Stb_image.cpp

Missile.o : Missile.cpp Missile.hpp Vector3D.hpp PrismMesh.hpp Polygon.hpp
	g++ -std=c++11 -c -w Missile.cpp

clean:
	rm *.o*
