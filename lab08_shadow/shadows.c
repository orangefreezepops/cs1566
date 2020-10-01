/*
 * Keegan Fouse
 * kdf11@pitt.edu 
 * 
 * Project 2: Viewing
 *
 * compile: > make
 * run: ./maze
 *
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "initShader.h"
#include "matrix_lib.h"

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include <string.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

double rad = (M_PI / 180);
vec4 vertices[98340];
vec4 colors[98340];
int num_vertices = 98340; //36 vetrices for cube + 98304 for the sphere
int v_index = 0;
mat4 frusty;

GLuint is_shadow_location;
GLuint light_location;
GLuint model_view_location;
mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
vec4 light_position = {0, 0, 0, 0};
vec4 vPosition = {0,0,0,0};
GLuint is_shadow;

// Vertices of a square

vec4 cube_vertices[36] = 
   {
	//cube front
	{-0.5, -0.5,  0.5, 1.0},	// bottom left
    { 0.5, -0.5,  0.5, 1.0},	// bottom right
    { 0.5,  0.5,  0.5, 1.0},	// top right
    {-0.5, -0.5,  0.5, 1.0},	// bottom left
    { 0.5,  0.5,  0.5, 1.0},	// top right
    {-0.5,  0.5,  0.5, 1.0},    // top left
	
	//cube right
	{ 0.5, 0.5,  0.5, 1.0},		// bottom left
    { 0.5, -0.5, 0.5, 1.0},		// bottom right
    { 0.5, -0.5, -0.5, 1.0},	// top right
    { 0.5, -0.5, -0.5, 1.0},		// bottom left
    { 0.5, 0.5, -0.5, 1.0},		// top right
    { 0.5, 0.5, 0.5, 1.0},   	// top left
	
	//cube left
	{ -0.5,  -0.5, 0.5, 1.0},	// bottom left
    { -0.5, 0.5, 0.5, 1.0},	// bottom right
    { -0.5,  -0.5, -0.5, 1.0},	// top right
    { -0.5,  -0.5, -0.5, 1.0},	// bottom left
    {-0.5, 0.5, 0.5, 1.0},	// top right
    { -0.5,  0.5, -0.5, 1.0},    // top left
	
	//cube back
	{0.5,  0.5, -0.5, 1.0},	// bottom left
    {0.5,  -0.5,  -0.5, 1.0},	// bottom right
    { -0.5,  -0.5,  -0.5, 1.0},	// top right
    {-0.5,  0.5, -0.5, 1.0},	// bottom left
    { 0.5,  0.5,  -0.5, 1.0},	// top right
    {-0.5, -0.5, -0.5, 1.0},    // top left
	
	//cube top
	{ 0.5,  0.5, 0.5, 1.0},	// bottom left
    {0.5,  0.5, -0.5, 1.0},	// bottom right
    {-0.5,  0.5,  -0.5, 1.0},	// top right
    {0.5, 0.5, 0.5, 1.0},	// bottom left
    {-0.5,  0.5,  -0.5, 1.0},	// top right
    { -0.5, 0.5,  0.5, 1.0},    // top left
	
	//cube bottom
	{-0.5, -0.5, 0.5, 1.0},	// bottom left
    { -0.5, -0.5, -0.5, 1.0},	// bottom right
    { 0.5, -0.5,  0.5, 1.0},	// top right
    {-0.5, -0.5, -0.5, 1.0},	// bottom left
    { 0.5, -0.5,  -0.5, 1.0},	// top right
    {0.5, -0.5,  0.5, 1.0}     // top left
	};


vec4 color(){
	//generates random RGB values for coloring 
	vec4 color;
	color.x = (float)rand() / (float)(RAND_MAX);
	color.y = (float)rand() / (float)(RAND_MAX);
	color.z = (float)rand() / (float)(RAND_MAX);
	color.w = 1.0;
	
	return color;
}

void triangle(vec4 top, vec4 bottom_right, vec4 bottom_left){
	mat4 temp = mult_matrix(translate(-0.5, 0.25, 0), scale(0.4, 0.4, 0.4));
	//incriment v_index through the normal and vertex arrays to normal the triangle
	vec4 curr = color();
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, top);
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, bottom_left);
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, bottom_right);
	
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
	subdivide(top, bottom_right, bottom_left, 6);
	
	//for the rest of the sides for the top half of the octohedron
	//rotate the face 90 degrees and then subdivide it into the 
	//smaller triangles as well
	for (int i = 0; i < 3; i++){

		bottom_left = matrix_x_vector(rotatey(90), bottom_left);
		bottom_right = matrix_x_vector(rotatey(90), bottom_right);

		subdivide(top, bottom_right, bottom_left, 6);
	}
	
	//triangle for bottom pyramid of octohedron
	vec4 bottom = {0.0, -1.0, 0.0, 1.0};
	vec4 upper_right = {1.0, 0.0, 0.0, 1.0};
	vec4 upper_left = {0.0, 0.0, 1.0, 1.0};
	
	subdivide(bottom, upper_left, upper_right, 6);
	
	//same process for the bottom half
	for (int i = 0; i < 3; i++){
	
		upper_left = matrix_x_vector(rotatey(90), upper_left);
		upper_right = matrix_x_vector(rotatey(90), upper_right);

		subdivide(bottom, upper_left, upper_right, 6);	
	}
}

void cube(){
	int i = 0;
	mat4 temp = mult_matrix(translate(0.5, 0.25, 0), scale(0.5, 0.5, 0.5));
	for (i = 0; i < 36; i ++){
	vec4 curr = color();
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[i]);
		colors[v_index] = curr;
		v_index++;
	}
}
void get_input(){
	float eyex, eyey, eyez;
	float atx, aty, atz;
	float lightx, lighty, lightz;
	char *input = malloc(sizeof(char));
	char *custom = "c";
	char *def = "d";
	frusty = frustum(-.5, .5, -.5, .5, -.5, .5);
	
	printf("Custom values (c)? or default (d)? ");
	scanf("%c", input);
	
	if (strcmp(input, custom) == 0){
		printf("Please enter an eye point: ");
		scanf("%f %f %f", &eyex, &eyey, &eyez);
	
		printf("Please enter a look-at point: ");
		scanf("%f %f %f", &atx, &aty, &atz);
	
		printf("Please enter a light location: ");
		scanf("%f %f %f", &lightx, &lighty, &lightz);
		
		vec4 eye = {eyex, eyey, eyez, 1};
		vec4 at = {atx, aty, atz, 1};
		vec4 light = {lightx, lighty, lightz, 1};
		vec4 up = {0, 1, 0, 1};
	
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
		return;
		
	} else if (strcmp(input, def) == 0){
		eyex = 0;
		eyey = 1;
		eyez = 0.75;
		
		atx = 0;
		aty = 0;
		atz = 0;
		
		lightx = 2;
		lighty = 3;
		lightz = 1;	
		
		vec4 eye = {eyex, eyey, eyez, 1};
		vec4 at = {atx, aty, atz, 1};
		vec4 light = {lightx, lighty, lightz, 1};
		light_position = light;
		vec4 up = {0, 1, 0, 1};
	
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
		return;
		
	} else {
		printf("Invalid input, lets redo that\n");
		free(input);
		get_input();
	}	
}

void apply_shadow(){
	is_shadow = 1;
}


void init(void)
{	
	sphere();
	cube();
	get_input();
	apply_shadow();

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
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
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

    glEnable(GL_CULL_FACE);
	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	light_location = glGetUniformLocation(program, "light_position");
	is_shadow_location = glGetUniformLocation(program, "is_shadow");
	
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
	
	glUniform1i(is_shadow_location, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
	glUniform4fv(light_location, 1, (GLfloat *) &light_position);
	
	glUniform1i(is_shadow_location, 1);
	
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Shadows");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
