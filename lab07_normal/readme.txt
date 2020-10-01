Keegan Fouse
kdf11@pitt.edu
10/18/19

included:
	fshade_ctm.glsl
	vshader_ctm.glsl
	initShader.h
	initShader.c
	matrix_lib.h
	matrix_lib.c
	sphere.c
	makefile
	
No special requirements, just:

	compile: 	> make
	run: 		> ./sphere
	
nothing is different in any of the shader files from the other
labs. I added rotation matrices, a vector normalize, an identity matrix,
and a vector magnitude methods in the matrix_lib files

sphere.c: the only drastically changed file, it builds a sphere
by initially building a octohedron with triangles, it then recursively 
divides each of the triangles on the octohedron into 4 more triangles per 
recursive call. After I recursed to a good amount of sub triangles, I normalized each
of the points generated in the sub trinagles about the radius to put the points on the 
surface of a sphere

the motion function and mouse function compute the points from mouse events
then compute the z coordinate by manipulating the formulas given in class to put them in 3 dimensions. the mouse just zooms on scroll functions. The motion function uses the fomula in the slides to create matrices for x and y rotations about thetax and thetay
there are comments throughout the code to guide and explain.

there are comments through out the document to explain

the program runs without error, but the rotation gets a little wonky about the y axis

