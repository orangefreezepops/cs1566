/*
 * Keegan Fouse
 * kdf11@pitt.edu 
 * 
 * Project 1: zoom and trackball animation on a 3D object
 * sphere.c
 *
 * compile: > make
 * run: ./sphere
 *
 *  Adopted From: Thumrongsak Kosiyatrakul's triangle.c
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "initShader.h"
#include "matrix_lib.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define bool int

#define false 0
#define true 1

/*my methods*/
	
double rad = (M_PI / 180);
vec4 vertices[24576];
vec4 colors[24576];
int num_vertices = 24576; //(8 sides of octohedron) * (1024 triangles per side) * (3 vertices) = 24576

GLuint ctm_location;
mat4 ctm;

//for zoom functionality
GLfloat sx = 1.0;
GLfloat sy = 1.0;
GLfloat sz = 1.0;

//globals for trackball animation
GLfloat x_val = 0;
GLfloat y_val = 0;
vec4 old = {0.0, 0.0, 0.0, 1.0};


vec4 color(){
	//generates random RGB values for coloring 
	vec4 color;
	color.x = (float)rand() / (float)(RAND_MAX);
	color.y = (float)rand() / (float)(RAND_MAX);
	color.z = (float)rand() / (float)(RAND_MAX);
	color.w = 1.0;
	
	return color;
}

//vertex and color arrays' placeholder
int v_index = 0;

void triangle(vec4 top, vec4 bottom_right, vec4 bottom_left){
	
	//incriment v_index through the color and vertex arrays to color the triangle
	vec4 curr = color();
	colors[v_index] = curr;
	vertices[v_index++] = top;
	colors[v_index] = curr;
	vertices[v_index++] = bottom_left;
	colors[v_index] = curr;
	vertices[v_index++] = bottom_right;
	
}

void subdivide(vec4 v1, vec4 v2, vec4 v3, int level){
	
	//base case... draw the triangle
	if (level == 0){
		triangle(v1, v2, v3);
	
	//tail recursive case
	} else {
		/* calculating midpoint of edges */
		//edge from v1 to v2
		float v12x = 0.5 * (v1.x + v2.x);
		float v12y = 0.5 * (v1.y + v2.y);
		float v12z = 0.5 * (v1.z + v2.z);
		
		vec4 v12 = {v12x, v12y, v12z, 0}; //put the point into a vector
		
		//normalize them to put points on the sphere
		v12 = normalize(v12);
		//the normalize messed with the w point so change it back to 1.0
		v12.w = 1.0;
		
		//edge from v1 to v3
		float v13x = 0.5 * (v1.x + v3.x);
		float v13y = 0.5 * (v1.y + v3.y);
		float v13z = 0.5 * (v1.z + v3.z);
		
		vec4 v13 = {v13x, v13y, v13z, 0}; //put the points into a vector
		
		//normalize them to put points on the sphere
		v13 = normalize(v13);
		v13.w = 1.0;
		
		//edge from v2 to v3
		float v23x = 0.5 * (v3.x + v2.x);
		float v23y = 0.5 * (v3.y + v2.y);
		float v23z = 0.5 * (v3.z + v2.z);
		
		vec4 v23 = {v23x, v23y, v23z, 0}; //put the points into a vector
		
		//normalize them to put points on the sphere
		v23 = normalize(v23);
		v23.w = 1.0;

		/* recurse */
		subdivide(v1, v12, v13, level - 1);
		subdivide(v12, v2, v23, level - 1);
		subdivide(v13, v23, v3, level - 1);
		subdivide(v12, v23, v13, level - 1);
	}	
}

void sphere(){
	
	/*Building the Octohedron*/
	
	/*triangle for top pyramid*/
	vec4 top = {0.0, 1.0, 0.0, 1.0};
	vec4 bottom_right = {1.0, 0.0, 0.0, 1.0};
	vec4 bottom_left = {0.0, 0.0, 1.0, 1.0};
	
	//subdivide the first side of the octohedron
	subdivide(top, bottom_right, bottom_left, 5);
	
	//for the rest of the sides for the top half of the octohedron
	//rotate the face 90 degrees and then subdivide it into the 
	//smaller triangles as well
	for (int i = 0; i < 3; i++){

		bottom_left = matrix_x_vector(rotatey(90), bottom_left);
		bottom_right = matrix_x_vector(rotatey(90), bottom_right);

		subdivide(top, bottom_right, bottom_left, 5);
	}
	
	//triangle for bottom pyramid of octohedron
	vec4 bottom = {0.0, -1.0, 0.0, 1.0};
	vec4 upper_right = {1.0, 0.0, 0.0, 1.0};
	vec4 upper_left = {0.0, 0.0, 1.0, 1.0};
	
	subdivide(bottom, upper_left, upper_right, 5);
	
	//same process for the bottom half
	for (int i = 0; i < 3; i++){
	
		upper_left = matrix_x_vector(rotatey(90), upper_left);
		upper_right = matrix_x_vector(rotatey(90), upper_right);

		subdivide(bottom, upper_left, upper_right, 5);	
	}
}

void motion(int x, int y){
	vec4 orthogonal;
	float result;
	float dx;
	float dy;
	float dz;
	float scale_x;
	float scale_y;
	float scale_z;
	int prev, curr;
	float theta;

	//scale x
	if (x <255.5){
		scale_x = -(float)((x / 255.5) - 1);
	} else {
		scale_x = -(float)((x - 255.5) / 255.5);
	}
	
	//scale y
	if (y <255.5){
		scale_y = (float)(-1*((y / 255.5) - 1));
	} else {
		scale_y = (float)(-1*((y - 255.5) / 255.5));
	}
	
	//printf("%f, %f\n", scale_x, scale_y);
	
	//determine z
	if(pow(scale_x, 2) + pow(scale_y, 2) <= 1) {
		scale_z = (float)sqrt(1 - pow(scale_x, 2) - pow(scale_y, 2));	
	} else {
		return;
	}
	
	vec4 new = {scale_x, scale_y, scale_z, 1.0}; //new position
	
	//check the difference between new spot and old spot
	if (new.x != old.x || new.y != old.y || new.z != old.z){
	
		vec4 otemp = cross_product(old, new);
		orthogonal = normalize(otemp);
		
		float d = sqrt(pow(orthogonal.y, 2) + pow(orthogonal.z, 2));
		
		float sin_thetax = orthogonal.y / d;
		float cos_thetax = orthogonal.z / d;
		float sin_thetay = orthogonal.x;
		float cos_thetay = d;
		
		//matrices for rotating about arbitrary axis
		mat4 rx_thetax = {{1, 0, 0, 0}, {0, cos_thetax, sin_thetax, 0}, {0, -sin_thetax, cos_thetax, 0}, {0, 0, 0, 1}};
		
		mat4 rx_negthetax = {{1, 0, 0, 0}, {0, cos_thetax, -sin_thetax, 0}, {0, sin_thetax, cos_thetax, 0}, {0, 0, 0, 1}};
		
		mat4 ry_thetay = {{cos_thetay, 0, -sin_thetay, 0}, {0, 1, 0, 0}, {sin_thetay, 0, cos_thetay, 0}, {0, 0, 0, 1}};
		
		mat4 ry_negthetay = {{cos_thetay, 0, sin_thetay, 0}, {0, 1, 0, 0}, {-sin_thetay, 0, cos_thetay, 0}, {0, 0, 0, 1}};
		
		result = dot_product(new, old);
		result /= magnitude(new);
		result /= magnitude(old);
		
		if (result > 1){
			theta = acos(-1);
		} else if (result < -1) {
			theta = acos(1);
		} else {
			theta = -acos(result) * (180 / M_PI);
		}
		
		mat4 t = translate(scale_x, scale_y, scale_z);
		mat4 t_inv = translate(-1*scale_x, -1*scale_y, -1*scale_z);
		
		// rx_neg * ry_neg * rz * ry * rx * ctm
		
		mat4 temp;
		temp = mult_matrix(rx_negthetax, ry_negthetay);
		temp = mult_matrix(temp, rotatez(theta));
		temp = mult_matrix(temp, ry_thetay);
		temp = mult_matrix(temp, rx_thetax);
		temp = mult_matrix(temp, scale(sx, sy, sz));
		ctm = mult_matrix(temp, ctm);
	}

	old.x = new.x;
	old.y = new.y;
	old.z = new.z;
	
	glutPostRedisplay();
	
}

void mouse(int button, int state, int x, int y){
	
	float scale_x;
	float scale_y;
	float scale_z;
	
	//I dont have a mouse, or zoom functionality on my trackpad
	//but I do have a touch screen
	//to zoom in its a reverse pinch, to zoom out its a pinch
	if (button == 4){
		sx /= 1.02;
		sy /= 1.02;
		sz /= 1.02;
		ctm = scale(sx, sy, sz);
	}
	if (button == 3){
		sx *= 1.02;
		sy *= 1.02;
		sz *= 1.02;
		ctm = scale(sx, sy, sz);
	}
	
	if(button == GLUT_LEFT_BUTTON){
		//scale x
		if (x <255.5){
			scale_x = -(float)((x / 255.5) - 1);
		} else {
			scale_x = -(float)((x - 255.5) / 255.5);
		}
	
		//scale y
		if (y <255.5){
			scale_y = (float)(-1*((y / 255.5) - 1));
		} else {
			scale_y = (float)(-1*((y - 255.5) / 255.5));
		}
	
		//determine z
		if(pow(scale_x, 2) + pow(scale_y, 2) <= 1) {
			scale_z = (float)sqrt(1 - pow(scale_x, 2) - pow(scale_y, 2));	
		} else {
			return;
		}
	}
	vec4 temp = {scale_x, scale_y, scale_z, 1.0}; //new position
	old = temp;
	glutPostRedisplay();
}

/*Dr. Tan's code*/

void init(void)
{
	sphere();
	ctm = identity();
	
    GLuint program = initShader("vshader_ctm.glsl", "fshader_ctm.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

	ctm_location = glGetUniformLocation(program, "ctm");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);
	/* 
		I also added functions here to zoom with the z and x keys
		to make it easier than the screen pinch
	*/

	//zoom out
	if (key == 'z'){
		sx /= 1.02;
		sy /= 1.02;
		sz /= 1.02;
		ctm = scale(sx, sy, sz);
	}
	//zoom in
	if (key == 'x'){
		sx *= 1.02;
		sy *= 1.02;
		sz *= 1.02;
		ctm = scale(sx, sy, sz);
	}

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Sphere");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}
