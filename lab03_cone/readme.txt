Keegan FOuse 
kdf11@pitt.edu
9/27/19

included:
	matrix_lib.h
	matrix_lib.c
	cone.c
	initshader.h
	initshader.c
	makefile
	readme
	vshader.glsl
	fshader.glsl 
	
compile:
	$ make
run:
	$ ./cone

The only two methods I wrote were color() and Cone(), the rest
were written and implemented by Dr. Tan for the triangle lab.

color() - creates a vector of random colors and returns that vector

cone() - generates a circular base for the cone using 3 vectors to 
define a triangle all centered around the origin and then builds the 
triangles side by side using trig functions to create the next triangle
in the sequence until the total angle = 360 degrees and forms a circle

it then creates the cone object on top of the circle by keeping the bottom two 
vertexes planted on the origin, but the top vertex at y=1, then using the same trig
functions builds side by side until a 360 degree path leads back and it becomes
a circular base all meeting at the point of the cone.