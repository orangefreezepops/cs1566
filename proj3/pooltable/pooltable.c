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
vec4 vertices[589860];
int num_vertices = 589860; //(20*20 floor tiles)*36 vertices + (98304 sphere vertices * 6 spheres) = 604224
vec4 colors[589860];
int v_index = 0;
mat4 frusty;

double degree = M_PI/2;
int ball_movement = 0;
float green_degree = 0;
float blue_degree = 0;
float yellow_degree = 0;
float orange_degree = 0;

float sx = 1;
float sy = 1;
float sz = 1;

float lightx = 0;
float lighty = 3;
float lightz = 0;	

typedef struct
{
	vec4 reflect_ambient;
	vec4 reflect_diffuse;
	vec4 reflect_specular;
	GLfloat shininess;
} material;

mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 projection_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

vec4 light_position = {0, 0, 0, 0};
vec4 vPosition = {0,0,0,0};
vec4 eyePosition = {0, 3, 8, 1};

vec4 ambient_product, diffuse_product, specular_product;

vec4 light_diffuse = {1, 1, 1, 1};
vec4 light_specular = {1, 1, 1, 1};
vec4 light_ambient ={1, 1, 1, 1}; 

vec4 regular_ball_scale = {0.5, 0.5, 0.5, 1};
vec4 light_ball_scale = {0.3, 0.3, 0.3, 1};

GLuint ctm_location;
GLuint is_shadow;
GLuint model_view_location;
GLuint projection_location;
GLuint ambient_product_location;
GLuint diffuse_product_location;
GLuint specular_product_location;
GLuint light_location;
GLuint shininess_location;
GLuint attenuation_constant_location;
GLuint attenuation_linear_location;
GLuint attenuation_quadratic_location;
GLuint is_shadow_location;

mat4 red_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 green_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 blue_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 yellow_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 orange_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 light_source_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 table_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

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
	
material materials[5] = {
	{{1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}, 10}, //red
	{{0, 1, 0, 1}, {0, 1, 0, 1}, {1, 1, 1, 1}, 10},	//green
	{{0, 0, 1, 1}, {0, 0, 1, 1}, {1, 1, 1, 1}, 10},	//blue
	{{1, 1, 0, 1}, {1, 1, 0, 1}, {1, 1, 1, 1}, 10},	//yellow
	{{1, 0.5, 0, 1}, {1, 0.5, 0, 1}, {1, 1, 1, 1}, 10} //orange
};

vec4 red_color = {1, 0, 0, 1};
vec4 green_color = {0, 1, 0, 1};
vec4 blue_color = {0, 0, 1, 1};
vec4 yellow_color = {1, 1, 0, 0};
vec4 orange_color = {1, 0.647, 0, 1};
vec4 white_color = {1, 1, 1, 1};
vec4 floor_color = {0.18, 0.31, 0.18, 1};

	
vec4 red_ball_location = {0.0, 0.5, 0.0, 1.0};
vec4 green_ball_location = {1.0, 0.5, 0.0, 1.0};
vec4 blue_ball_location = {2.0, 0.5, 0.0, 1.0};
vec4 yellow_ball_location = {3.0, 0.5, 0.0, 1.0};
vec4 orange_ball_location = {4.0, 0.5, 0.0, 1.0};
vec4 light_sphere_location = {0.0, 3.0, 0.0, 1.0};

void normal(){
	int i;
	for(i = 0; i < 589860; i+= 3)
	{
		vec4 v1 = sub_vec(vertices[i + 1], vertices[i]);
		vec4 v2 = sub_vec(vertices[i + 2], vertices[i + 1]);
		vec4 n = normalize(cross_product(v1, v2));
		colors[i] = n;
		colors[i + 1] = n;
		colors[i + 2] = n;
	}
}

void fill_floor(){
	int i, j, k, posx, posz = 0;
	mat4 temp;
	temp = mult_matrix(translate(0, 0, 0), scale(20, 0.005, 20));
	for (k = 0; k < 36; k++){
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
		colors[v_index] = floor_color;
		v_index++;
	}
}

vec4 color(char *color_name){
	//generates random RGB values for coloring 
	char *red = "red";
	char *green = "green";
	char *blue = "blue";
	char *yellow = "yellow";
	char *orange = "orange";
	char *white = "white";
	char *floor = "floor";
	
	int result_red = strcmp(color_name, red);
	int result_green = strcmp(color_name, green);
	int result_blue = strcmp(color_name, blue);
	int result_yellow = strcmp(color_name, yellow);
	int result_orange = strcmp(color_name, orange);
	int result_white = strcmp(color_name, white);
	int result_floor = strcmp(color_name, floor);
	
	if (!result_red){
		return red_color;
	}
	
	if (!result_green){
		return green_color;
	}
	
	if (!result_blue){
		return blue_color;
	}
	
	if (!result_yellow){
		return yellow_color;
	}
	
	if (!result_orange){
		return orange_color;
	}
	
	if (!result_white){
		return white_color;
	}
	
	if(!result_floor){
		return floor_color;
	}
}

void triangle(vec4 top, vec4 bottom_right, vec4 bottom_left, vec4 pos, vec4 size, char *c){
	mat4 temp = mult_matrix(translate(pos.x, pos.y, pos.z), scale(size.x, size.y, size.z));
	//incriment v_index through the normal and vertex arrays to normal the triangle
	vec4 curr = color(c);
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, top);
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, bottom_left);
	colors[v_index] = curr;
	vertices[v_index++] = matrix_x_vector(temp, bottom_right);
	
}

void subdivide(vec4 v1, vec4 v2, vec4 v3, int level, vec4 pos, vec4 size, char *color){
	
	//base case... draw the triangle
	if (level == 0){
		triangle(v1, v2, v3, pos, size, color);
	
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
		subdivide(v1, v12, v13, level - 1, pos, size, color);
		subdivide(v12, v2, v23, level - 1, pos, size, color);
		subdivide(v13, v23, v3, level - 1, pos, size, color);
		subdivide(v12, v23, v13, level - 1, pos, size, color);
	}	
}

void sphere(vec4 pos, vec4 size, char *color){
	
	/*Building the Octohedron*/
	/*triangle for top pyramid*/
	vec4 top = {0.0, 1.0, 0.0, 1.0};
	vec4 bottom_right = {1.0, 0.0, 0.0, 1.0};
	vec4 bottom_left = {0.0, 0.0, 1.0, 1.0};
	
	//subdivide the first side of the octohedron
	subdivide(top, bottom_right, bottom_left, 6, pos, size, color);
	
	//for the rest of the sides for the top half of the octohedron
	//rotate the face 90 degrees and then subdivide it into the 
	//smaller triangles as well
	for (int i = 0; i < 3; i++){

		bottom_left = matrix_x_vector(rotatey(90), bottom_left);
		bottom_right = matrix_x_vector(rotatey(90), bottom_right);

		subdivide(top, bottom_right, bottom_left, 6, pos, size, color);
	}
	
	//triangle for bottom pyramid of octohedron
	vec4 bottom = {0.0, -1.0, 0.0, 1.0};
	vec4 upper_right = {1.0, 0.0, 0.0, 1.0};
	vec4 upper_left = {0.0, 0.0, 1.0, 1.0};
	
	subdivide(bottom, upper_left, upper_right, 6, pos, size, color);
	
	//same process for the bottom half
	for (int i = 0; i < 3; i++){
	
		upper_left = matrix_x_vector(rotatey(90), upper_left);
		upper_right = matrix_x_vector(rotatey(90), upper_right);

		subdivide(bottom, upper_left, upper_right, 6, pos, size, color);	
	}
}

void place_balls(){
	//red ball
	sphere(red_ball_location, regular_ball_scale, "red");
	//green ball
	sphere(green_ball_location, regular_ball_scale, "green");
	//blue ball
	sphere(blue_ball_location, regular_ball_scale, "blue");
	//yellow ball
	sphere(yellow_ball_location, regular_ball_scale, "yellow");
	//orange ball
	sphere(orange_ball_location, regular_ball_scale, "orange");
	//light sphere
	sphere(light_sphere_location, light_ball_scale, "white");
	
}
void set_stage(){
	float eyex, eyey, eyez;
	float atx, aty, atz;
	frusty = frustum(-.5, .5, -.5, .5, -.5, .5);
	
		eyex = 0;
		eyey = 3;
		eyez = 8;
		
		atx = 0;
		aty = 0;
		atz = 0;
		
		vec4 eye = {eyex, eyey, eyez, 1};
		vec4 at = {atx, aty, atz, 1};
		vec4 light = {lightx, lighty, lightz, 1};
		vec4 up = {0, 1, 0, 1};
		
		light_position = light;
		eyePosition = eye;
	
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));	
}

void apply_shadow(){
	is_shadow = 1;
}


void init(void)
{	
	set_stage();
	fill_floor();
	place_balls();
	normal();
	apply_shadow();

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
	ctm_location = glGetUniformLocation(program, "ctm");

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
	projection_location = glGetUniformLocation(program, "projection_matrix");
	light_location = glGetUniformLocation(program, "light_position");
	is_shadow_location = glGetUniformLocation(program, "is_shadow");
	
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
	
	//draw table, no shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &table_ctm);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	//draw red ball with shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &red_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 36, 98304);
	
	glUniform1i(is_shadow_location, 1);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &red_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 36, 98304);
	
	//draw green ball with shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &green_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 98340, 98304);
	
	glUniform1i(is_shadow_location, 1);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &green_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 98340, 98304);
	
	//draw blue ball with shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &blue_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 196644, 98304);
	
	glUniform1i(is_shadow_location, 1);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &blue_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 196644, 98304);
	
	//draw yellow ballwith shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &yellow_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 294948, 98304);
	
	glUniform1i(is_shadow_location, 1);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &yellow_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 294948, 98304);
	
	//draw orange ball with shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &orange_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 393252, 98304);
	
	glUniform1i(is_shadow_location, 1);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &orange_ball_ctm);
	glDrawArrays(GL_TRIANGLES, 393252, 98304);
	
	//draw light ball, no shadow
	glUniform1i(is_shadow_location, 0);
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &light_source_ctm);
	glDrawArrays(GL_TRIANGLES, 491556, 98304);
	
	glUniform4fv(light_location, 1, (GLfloat *) &light_position);
	
    glutSwapBuffers();
}

void idle(void){
	if (ball_movement == 1){
		mat4 tempred;
		mat4 tempgreen;
		mat4 tempblue;
		mat4 tempyellow;
		mat4 temporange;
		
		tempred = mult_matrix(scale(1, 1, 1), translate(0,0,0));
		red_ball_ctm = tempred;
	
		tempgreen = mult_matrix(rotatey(green_degree), translate(0, 0, 0));
		green_ball_ctm = tempgreen;
	
		tempblue = mult_matrix(rotatey(blue_degree), translate(0, 0, 0));
		blue_ball_ctm = tempblue;
		
		tempyellow = mult_matrix(rotatey(yellow_degree), translate(0, 0, 0));
		yellow_ball_ctm = tempyellow;
		
		temporange = mult_matrix(rotatey(orange_degree), translate(0, 0, 0));
		orange_ball_ctm = temporange;
	
		green_degree += 1.3;
		blue_degree += 0.45;
		yellow_degree += 2;
		orange_degree += 0.7;
	
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'r')
    	exit(0);
	
	if(key == 'b')
		ball_movement = 1;
	
	//zoom out
	if (key == 'o'){
		//vec4 newEye
		sx /= 1.02;
		sy /= 1.02;
		sz /= 1.02;
		mat4 temp = model_view_matrix;
		model_view_matrix = mult_matrix(temp, scale(sx, sy, sz));
	}
	//zoom in
	if (key == 'z'){
		sx *= 1.02;
		sy *= 1.02;
		sz *= 1.02;
		mat4 temp = model_view_matrix;
		model_view_matrix = mult_matrix(temp, scale(sx, sy, sz));
	}
	
	//move light source away from viewer
	if (key == 'q'){
		light_sphere_location.z -= 0.2;
		light_position.z -= 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
		
	}
	
	//move light source closer to viewer
	if (key == 'e'){
		light_sphere_location.z += 0.2;
		light_position.z += 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
	}
	
	//move light source right
	if (key == 'd'){
		light_sphere_location.x += 0.2;
		light_position.x += 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
	}
	
	//move light source left
	if (key == 'a'){
		light_sphere_location.x -= 0.2;
		light_position.x -= 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
	}
	
	//move light source away from table
	if (key == 'w'){
		light_sphere_location.y += 0.2;
		light_position.y += 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
	}
	
	//move light source closer to table
	if (key == 's'){
		light_sphere_location.y -= 0.2;
		light_position.y -= 0.2;
		light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
	}
	
	//circle right
	if (key == 'l'){
		degree -= 0.05;
		vec4 eye = {8*cos(degree), 3, 8*sin(degree), 1.0};
		vec4 at = {0, 0, 0, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
		
	}
	
	//circle left
	if (key == 'j'){
		degree += 0.05;
		vec4 eye = {8*cos(degree), 3, 8*sin(degree), 1.0};
		vec4 at = {0, 0, 0, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
	}
	
	//pan up
	if (key == 'i'){
		//degree += 0.05;
		vec4 eye = {8*cos(degree), 3*sin(degree), 8*sin(degree), 1.0};
		vec4 at = {0, 0, 0, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
	}
	
	//pan down
	if (key == 'k'){
	
	}

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
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
