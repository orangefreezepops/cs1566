/*
 * Keegan Fouse
 * kdf11@pitt.edu 
 * 
 * Project 4: rubiks cube
 * rubiks.c
 *
 * compile: > make
 * run: ./rubiks
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
#include "solve_rc.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define bool int

#define false 0
#define true 1

/*my methods*/
	
double rad = (M_PI / 180);
vec4 vertices[3564]; //(132 vertices per cube * 27 cubes) = 3564
vec4 normals[3564];
vec4 colors[3564];
int num_vertices = 3564;
int v_index;

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

int idle_front = 0;
int idle_right = 0;
int idle_back = 0;
int idle_left = 0;
int idle_up = 0;
int idle_down = 0;
double rotation = 0;
double f_rotation = 0;
double r_rotation = 0;
double b_rotation = 0;
double l_rotation = 0;
double u_rotation = 0;
double d_rotation = 0;
double degree = 0.05;


typedef struct
{
	vec4 reflect_ambient;
	vec4 reflect_diffuse;
	vec4 reflect_specular;
	GLfloat shininess;
} material;

mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 projection_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
vec4 vPosition = {0, 0, 0, 0};
vec4 vNormal = {0, 0, 0, 0};
vec4 ambient_product = {0, 0, 0, 0};
vec4 diffuse_product = {0, 0, 0, 0};
vec4 specular_product = {0, 0, 0, 0};
vec4 light_diffuse = {1, 1, 1, 1};
vec4 light_specular = {1, 1, 1, 1};
vec4 light_ambient ={0.2, 0.2, 0.2, 1}; 

//int ctm_location[27] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

GLuint model_view_location;
GLuint projection_location;
GLuint ambient_product_location;
GLuint diffuse_product_location;
GLuint specular_product_location;
GLuint light_location;
GLuint attenuation_constant_location;
GLuint attenuation_linear_location;
GLuint attenuation_quadratic_location;
GLuint shininess_location;

vec4 light_position = {0, 0, 5, 1};
vec4 eye = {0, 0, 5, 1};
vec4 at = {0, 0, 0, 1};
vec4 up = {0, 1, 0, 0};

vec4 cube_positions[27] = {
{-0.6, 0.6, 0.6, 1},
{ 0,   0.6, 0.6, 1},
{ 0.6, 0.6, 0.6, 1},
{-0.6, 0,   0.6, 1},
{ 0,   0,   0.6, 1},
{ 0.6, 0,   0.6, 1},
{-0.6,-0.6, 0.6, 1},
{ 0,  -0.6, 0.6, 1},
{ 0.6,-0.6, 0.6, 1},

{-0.6, 0.6, 0, 1},
{ 0, 0.6, 0, 1},
{0.6, 0.6, 0, 1},
{-0.6, 0, 0, 1},
{0, 0, 0, 1},
{0.6, 0, 0, 1},
{-0.6, -0.6, 0, 1},
{0, -0.6, 0, 1},
{0.6, -0.6, 0, 1},

{-0.6, 0.6, -0.6, 1},
{0, 0.6, -0.6, 1},
{0.6, 0.6, -0.6, 1},
{-0.6, 0, -0.6, 1},
{0, 0, -0.6, 1},
{0.6, 0, -0.6, 1},
{-0.6, -0.6, -0.6, 1},
{0, -0.6, -0.6, 1},
{0.6, -0.6, -0.6, 1}
};


vec4 cube_vertices[132] = 
   {
	//cube front
	{-0.5, -0.5,  0.6, 1.0},	// bottom left
    { 0.5, -0.5,  0.6, 1.0},	// bottom right
    { 0.5,  0.5,  0.6, 1.0},	// top right
    {-0.5, -0.5,  0.6, 1.0},	// bottom left
    { 0.5,  0.5,  0.6, 1.0},	// top right
    {-0.5,  0.5,  0.6, 1.0},    // top left

	//cube right
	{ 0.6,  0.5,  0.5, 1.0},	// bottom left
    { 0.6, -0.5,  0.5, 1.0},	// bottom right
    { 0.6, -0.5, -0.5, 1.0},	// top right
    { 0.6, -0.5, -0.5, 1.0},	// bottom left
    { 0.6,  0.5, -0.5, 1.0},	// top right
    { 0.6,  0.5,  0.5, 1.0},   	// top left
	
	//cube back
	{ 0.5,  0.5, -0.6, 1.0},	// bottom left
    { 0.5, -0.5, -0.6, 1.0},	// bottom right
    {-0.5, -0.5, -0.6, 1.0},	// top right
    {-0.5,  0.5, -0.6, 1.0},	// bottom left
    { 0.5,  0.5, -0.6, 1.0},	// top right
    {-0.5, -0.5, -0.6, 1.0},    // top left
	
	//cube left
	{-0.6, -0.5,  0.5, 1.0},	// bottom left
    {-0.6,  0.5,  0.5, 1.0},	// bottom right
    {-0.6, -0.5, -0.5, 1.0},	// top right
    {-0.6, -0.5, -0.5, 1.0},	// bottom left
    {-0.6,  0.5,  0.5, 1.0},	// top right
    {-0.6,  0.5, -0.5, 1.0},    // top left
	
	//cube top
	{ 0.5,  0.6,  0.5, 1.0},	// bottom left
    { 0.5,  0.6, -0.5, 1.0},	// bottom right
    {-0.5,  0.6, -0.5, 1.0},	// top right
    { 0.5,  0.6,  0.5, 1.0},	// bottom left
    {-0.5,  0.6, -0.5, 1.0},	// top right
    {-0.5,  0.6,  0.5, 1.0},    // top left
	
	//cube bottom
	{-0.5, -0.6,  0.5, 1.0},	// bottom left
    {-0.5, -0.6, -0.5, 1.0},	// bottom right
    { 0.5, -0.6,  0.5, 1.0},	// top right
    {-0.5, -0.6, -0.5, 1.0},	// bottom left
    { 0.5, -0.6, -0.5, 1.0},	// top right
    { 0.5, -0.6,  0.5, 1.0},    // top left
	
	//triangle front top right
	{ 0.5,  0.5,  0.6, 1.0},	// bottom left
    { 0.6,  0.5,  0.5, 1.0},	// bottom right
    { 0.5,  0.6,  0.5, 1.0},	// top right
	
	//trianlge front bottom right
	{ 0.5, -0.6,  0.5, 1.0},	// bottom left
    { 0.6, -0.5,  0.5, 1.0},	// bottom right
    { 0.5, -0.5,  0.6, 1.0},	// top right
	
	//triangle front top left
	{-0.6,  0.5,  0.5, 1.0},	// bottom left
    {-0.5,  0.5,  0.6, 1.0},	// bottom right
    {-0.5,  0.6,  0.5, 1.0},	// top right
	
	//triangle front bottom left
	{-0.6, -0.5,  0.5, 1.0},	// bottom left
    {-0.5, -0.6,  0.5, 1.0},	// bottom right
    {-0.5, -0.5,  0.6, 1.0},	// top right
	
	//triangle back top right
	{ -0.5,  0.5, -0.6, 1.0},	// bottom left
    { -0.6,  0.5, -0.5, 1.0},	// bottom right
    { -0.5,  0.6, -0.5, 1.0},	// top right
	
	//trianlge back bottom right
	{-0.5, -0.6, -0.5, 1.0},	// bottom left
    {-0.6, -0.5, -0.5, 1.0},	// bottom right
    {-0.5, -0.5, -0.6, 1.0},	// top right
	
	//triangle back top left
	{ 0.6,  0.5, -0.5, 1.0},	// bottom left
    { 0.5,  0.5, -0.6, 1.0},	// bottom right
    { 0.5,  0.6, -0.5, 1.0},	// top right
	
	//triangle back bottom left
	{ 0.6, -0.5, -0.5, 1.0},	// bottom left
    { 0.5, -0.6, -0.5, 1.0},	// bottom right
    { 0.5, -0.5, -0.6, 1.0},	// top right
	
	//front top connection
	{-0.5,  0.5,  0.6, 1.0},	// bottom left
    { 0.5,  0.5,  0.6, 1.0},	// bottom right
    { 0.5,  0.6,  0.5, 1.0},	// top right
    {-0.5,  0.5,  0.6, 1.0},	// bottom left
    { 0.5,  0.6,  0.5, 1.0},	// top right
    {-0.5,  0.6,  0.5, 1.0},    // top left
	
	//front bottom connection
	{-0.5, -0.6,  0.5, 1.0},	// bottom left
    { 0.5, -0.6,  0.5, 1.0},	// bottom right
    { 0.5, -0.5,  0.6, 1.0},	// top right
    {-0.5, -0.6,  0.5, 1.0},	// bottom left
    { 0.5, -0.5,  0.6, 1.0},	// top right
    {-0.5, -0.5,  0.6, 1.0},    // top left
	
	//front right connection
	{ 0.5, -0.5,  0.6, 1.0},	// bottom left
    { 0.6, -0.5,  0.5, 1.0},	// bottom right
    { 0.6,  0.5,  0.5, 1.0},	// top right
    { 0.5, -0.5,  0.6, 1.0},	// bottom left
    { 0.6,  0.5,  0.5, 1.0},	// top right
    { 0.5,  0.5,  0.6, 1.0},    // top left
	
	//front left connection
	{-0.6, -0.5,  0.5, 1.0},	// bottom left
    {-0.5, -0.5,  0.6, 1.0},	// bottom right
    {-0.5,  0.5,  0.6, 1.0},	// top right
    {-0.6, -0.5,  0.5, 1.0},	// bottom left
    {-0.5,  0.5,  0.6, 1.0},	// top right
    {-0.6,  0.5,  0.5, 1.0},    // top left
	
	//right top connection
	{ 0.6,  0.5,  0.5, 1.0},	// bottom left
    { 0.6,  0.5, -0.5, 1.0},	// bottom right
    { 0.5,  0.6, -0.5, 1.0},	// top right
    { 0.6,  0.5,  0.5, 1.0},	// bottom left
    { 0.5,  0.6, -0.5, 1.0},	// top right
    { 0.5,  0.6,  0.5, 1.0},    // top left
	
	//right bottom connection
	{ 0.5, -0.6,  0.5, 1.0},	// bottom left
    { 0.5, -0.6, -0.5, 1.0},	// bottom right
    { 0.6, -0.5, -0.5, 1.0},	// top right
    { 0.5, -0.6,  0.5, 1.0},	// bottom left
    { 0.6, -0.5, -0.5, 1.0},	// top right
    { 0.6, -0.5,  0.5, 1.0},    // top left
	
	//right right connection
	{ 0.6, -0.5, -0.5, 1.0},	// bottom left
    { 0.5, -0.5, -0.6, 1.0},	// bottom right
    { 0.5,  0.5, -0.6, 1.0},	// top right
    { 0.6, -0.5, -0.5, 1.0},	// bottom left
    { 0.5,  0.5, -0.6, 1.0},	// top right
    { 0.6,  0.5, -0.5, 1.0},    // top left
	
	//back top connection
	{ 0.5,  0.5, -0.6, 1.0},	// bottom left
    {-0.5,  0.5, -0.6, 1.0},	// bottom right
    {-0.5,  0.6, -0.5, 1.0},	// top right
    { 0.5,  0.5, -0.6, 1.0},	// bottom left
    {-0.5,  0.6, -0.5, 1.0},	// top right
    { 0.5,  0.6, -0.5, 1.0},    // top left
	
	//back bottom connection
	{ 0.5, -0.6, -0.5, 1.0},	// bottom left
    {-0.5, -0.6, -0.5, 1.0},	// bottom right
    {-0.5, -0.5, -0.6, 1.0},	// top right
    { 0.5, -0.6, -0.5, 1.0},	// bottom left
    {-0.5, -0.5, -0.6, 1.0},	// top right
    { 0.5, -0.5, -0.6, 1.0},    // top left
	
	//back right connection
	{-0.5, -0.5, -0.6, 1.0},	// bottom left
    {-0.6, -0.5, -0.5, 1.0},	// bottom right
    {-0.6,  0.5, -0.5, 1.0},	// top right
    {-0.5, -0.5, -0.6, 1.0},	// bottom left
    {-0.6,  0.5, -0.5, 1.0},	// top right
    {-0.5,  0.5, -0.6, 1.0},    // top left
	
	//left top connection
	{-0.6,  0.5, -0.5, 1.0},	// bottom left
    {-0.6,  0.5,  0.5, 1.0},	// bottom right
    {-0.5,  0.6,  0.5, 1.0},	// top right
    {-0.6,  0.5, -0.5, 1.0},	// bottom left
    {-0.5,  0.6,  0.5, 1.0},	// top right
    {-0.5,  0.6, -0.5, 1.0},    // top left
	
	//left bottom connection
	{-0.5, -0.6, -0.5, 1.0},	// bottom left
    {-0.5, -0.6,  0.5, 1.0},	// bottom right
    {-0.6, -0.5,  0.5, 1.0},	// top right
    {-0.5, -0.6, -0.5, 1.0},	// bottom left
    {-0.6, -0.5,  0.5, 1.0},	// top right
    {-0.6, -0.5, -0.5, 1.0}     // top left
};

enum cube_sides {
	F_GREEN,
	R_RED,
	B_BLUE,
	L_ORANGE,
	T_WHITE,
	U_YELLOW,
	BLACK,
	NUM_MATERIALS
};
	
material materials[NUM_MATERIALS] = {
	{{0, 1, 0, 1}, {0, 1, 0, 1}, {1, 1, 1, 1}, 10},		//green
	{{1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}, 10}, 	//red
	{{0, 0, 1, 1}, {0, 0, 1, 1}, {1, 1, 1, 1}, 10},		//blue
	{{1, 0.5, 0, 1}, {1, 0.5, 0, 1}, {1, 1, 1, 1}, 10}, //orange
	{{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, 10}, 	//white
	{{1, 1, 0, 1}, {1, 1, 0, 1}, {1, 1, 1, 1}, 10}, 	//yellow
	{{0, 0, 0, 1}, {0, 0, 0, 1}, {1, 1, 1, 1}, 10}		//black
};

vec4 cube_colors[132] = {
	//green
	{0, 1, 0, 1},
	{0, 1, 0, 1},
	{0, 1, 0, 1},
	{0, 1, 0, 1},
	{0, 1, 0, 1},
	{0, 1, 0, 1},
	
	//red
	{1, 0, 0, 1},
	{1, 0, 0, 1},
	{1, 0, 0, 1},
	{1, 0, 0, 1},
	{1, 0, 0, 1},
	{1, 0, 0, 1},
	
	//blue
	{0, 0, 1, 1},
	{0, 0, 1, 1},
	{0, 0, 1, 1},
	{0, 0, 1, 1},
	{0, 0, 1, 1},
	{0, 0, 1, 1},
	
	//orange
	{1, 0.647, 0, 1},
	{1, 0.647, 0, 1},
	{1, 0.647, 0, 1},
	{1, 0.647, 0, 1},
	{1, 0.647, 0, 1},
	{1, 0.647, 0, 1},
	
	//white
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	{1, 1, 1, 1},
	
	//yellow
	{1, 1, 0, 1},
	{1, 1, 0, 1},
	{1, 1, 0, 1},
	{1, 1, 0, 1},
	{1, 1, 0, 1},
	{1, 1, 0, 1},
	
	//rest are black
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1},
	{0, 0, 0, 1}
};
mat4 ctm_arr[27] = {
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
};

mat4 swap_ctm[27] = {
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},
	{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
};

mat4 cube1_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube2_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube3_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube4_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube5_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube6_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube7_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube8_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube9_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

mat4 cube10_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube11_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube12_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube13_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube14_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube15_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube16_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube17_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube18_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

mat4 cube19_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube20_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube21_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube22_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube23_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube24_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube25_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube26_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 cube27_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	
void cube(vec4 position){
	int i = 0;
	for (i = 0; i < 132; i ++){
		mat4 temp = mult_matrix(translate(position.x, position.y, position.z), scale(0.3, 0.3, 0.3));
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[i]);
		colors[v_index] = cube_colors[i];
		v_index++;
	}
}

void normal(){
	int i;
	for(i = 0; i < 3564; i+= 3)
	{
		vec4 v1 = sub_vec(vertices[i + 1], vertices[i]);
		vec4 v2 = sub_vec(vertices[i + 2], vertices[i + 1]);
		vec4 n = normalize(cross_product(v1, v2));
		normals[i] = n;
		normals[i + 1] = n;
		normals[i + 2] = n;
	}
}

void place_cubes(){
	int i;
	
	for (i = 0; i < 27; i ++){
		cube(scalar_vec_mult(cube_positions[i], 0.6));
	}
}

void init(void)
{
	place_cubes();
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
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

    glEnable(GL_CULL_FACE);
	ctm_location = glGetUniformLocation(program, "ctm");
	ambient_product_location = glGetUniformLocation(program, "ambient_product");
	diffuse_product_location = glGetUniformLocation(program, "diffuse_product");
	specular_product_location = glGetUniformLocation(program, "specular_product");
	light_location = glGetUniformLocation(program, "light_position");
	attenuation_constant_location = glGetUniformLocation(program, "attenuation_constant");
	attenuation_linear_location = glGetUniformLocation(program, "attenuation_linear");
	attenuation_quadratic_location = glGetUniformLocation(program, "attenuation_quadratic");
	shininess_location = glGetUniformLocation(program, "shininess");
	
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
	/*glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
	glUniform1f(attenuation_constant_location, 0);
	glUniform1f(attenuation_linear_location, 0);
	glUniform1f(attenuation_quadratic_location, 0);
	glUniform4fv(light_location, 1, (GLfloat *) &light_position);
	*/
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[0]);
	glDrawArrays(GL_TRIANGLES, 0, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[1]);
	glDrawArrays(GL_TRIANGLES, 132, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[2]);
	glDrawArrays(GL_TRIANGLES, 264, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[3]);
	glDrawArrays(GL_TRIANGLES, 396, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[4]);
	glDrawArrays(GL_TRIANGLES, 528, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[5]);
	glDrawArrays(GL_TRIANGLES, 660, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[6]);
	glDrawArrays(GL_TRIANGLES, 792, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[7]);
	glDrawArrays(GL_TRIANGLES, 924, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[8]);
	glDrawArrays(GL_TRIANGLES, 1056, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[9]);
	glDrawArrays(GL_TRIANGLES, 1188, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[10]);
	glDrawArrays(GL_TRIANGLES, 1320, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[11]);
	glDrawArrays(GL_TRIANGLES, 1452, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[12]);
	glDrawArrays(GL_TRIANGLES, 1584, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[13]);
	glDrawArrays(GL_TRIANGLES, 1716, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[14]);
	glDrawArrays(GL_TRIANGLES, 1848, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[15]);
	glDrawArrays(GL_TRIANGLES, 1980, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[16]);
	glDrawArrays(GL_TRIANGLES, 2112, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[17]);
	glDrawArrays(GL_TRIANGLES, 2244, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[18]);
	glDrawArrays(GL_TRIANGLES, 2376, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[19]);
	glDrawArrays(GL_TRIANGLES, 2508, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[20]);
	glDrawArrays(GL_TRIANGLES, 2640, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[21]);
	glDrawArrays(GL_TRIANGLES, 2772, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[22]);
	glDrawArrays(GL_TRIANGLES, 2904, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[23]);
	glDrawArrays(GL_TRIANGLES, 3036, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[24]);
	glDrawArrays(GL_TRIANGLES, 3168, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[25]);
	glDrawArrays(GL_TRIANGLES, 3300, 132);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_arr[26]);
	glDrawArrays(GL_TRIANGLES, 3432, 132);
	
    glutSwapBuffers();
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
		
		int i;
		for(i = 0; i < 27; i ++){
			ctm_arr[i] = mult_matrix(temp, ctm_arr[i]);
		}
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

void swap(char move){
	int i;
	
	//front rotation
	if(move == 'f'){
		ctm_arr[0] = swap_ctm[2];
		ctm_arr[1] = swap_ctm[5];
		ctm_arr[2] = swap_ctm[8];
		
		ctm_arr[3] = swap_ctm[1];
		ctm_arr[4] = swap_ctm[4];
		ctm_arr[5] = swap_ctm[7];
		
		ctm_arr[6] = swap_ctm[0];
		ctm_arr[7] = swap_ctm[3];
		ctm_arr[8] = swap_ctm[6];
	}
	
	/*right rotation
	if(move == 'r'){
		ctm_arr[2] = swap_ctm[20];
		ctm_arr[5] = swap_ctm[11];
		ctm_arr[8] = swap_ctm[2];
		
		ctm_arr[11] = swap_ctm[23];
		ctm_arr[14] = swap_ctm[14];
		ctm_arr[17] = swap_ctm[5];
		
		ctm_arr[20] = swap_ctm[26];
		ctm_arr[23] = swap_ctm[17];
		ctm_arr[26] = swap_ctm[8];
	}
	
	//back rotation
	if(move == 'b'){
		ctm_arr[18] = swap_ctm[20];
		ctm_arr[19] = swap_ctm[23];
		ctm_arr[20] = swap_ctm[26];
		
		ctm_arr[21] = swap_ctm[19];
		ctm_arr[22] = swap_ctm[22];
		ctm_arr[23] = swap_ctm[25];
		
		ctm_arr[24] = swap_ctm[18];
		ctm_arr[25] = swap_ctm[21];
		ctm_arr[26] = swap_ctm[24];
	}
	
	//left rotation
	if(move == 'l'){
		ctm_arr[0] = swap_ctm[6];
		ctm_arr[9] = swap_ctm[3];
		ctm_arr[18] = swap_ctm[0];
		
		ctm_arr[3] = swap_ctm[15];
		ctm_arr[12] = swap_ctm[12];
		ctm_arr[21] = swap_ctm[79];
		
		ctm_arr[6] = swap_ctm[24];
		ctm_arr[15] = swap_ctm[21];
		ctm_arr[24] = swap_ctm[18];
	}
	*/
	//up rotation
	if(move == 'u'){
		ctm_arr[0] = swap_ctm[18];
		ctm_arr[1] = swap_ctm[9];
		ctm_arr[2] = swap_ctm[0];
		
		ctm_arr[9] = swap_ctm[20];
		ctm_arr[10] = swap_ctm[10];
		ctm_arr[11] = swap_ctm[1];
		
		ctm_arr[18] = swap_ctm[20];
		ctm_arr[19] = swap_ctm[11];
		ctm_arr[20] = swap_ctm[2];
	}
	
	/*down rotation
	if(move == 'd'){
		ctm_arr[6] = swap_ctm[24];
		ctm_arr[7] = swap_ctm[15];
		ctm_arr[8] = swap_ctm[6];
		
		ctm_arr[15] = swap_ctm[25];
		ctm_arr[16] = swap_ctm[16];
		ctm_arr[17] = swap_ctm[7];
		
		ctm_arr[24] = swap_ctm[26];
		ctm_arr[25] = swap_ctm[17];
		ctm_arr[26] = swap_ctm[8];
	}
	*/
}

char *shuffle(){
	
}


void idle(void){
	int i;
	if (idle_front == 1){
		for (i = 0; i < 9; i ++){
			ctm_arr[i] = mult_matrix(ctm_arr[i], rotatez(degree));
			//swap_ctm[i] = ctm_arr[i];
		}
		f_rotation += degree;
		if (f_rotation > 89.999999){
			f_rotation = 0;
			idle_front = 0;
			
			//swap('f');
		}
		
	}
	
	if (idle_back == 1){
		for (i = 18; i < 27; i ++){
			ctm_arr[i] = mult_matrix(ctm_arr[i], rotatez(degree));
			//swap_ctm[i] = ctm_arr[i];
		}
		b_rotation += degree;
		if (b_rotation > 89.999999){
			b_rotation = 0;
			idle_back = 0;
			//swap('b');
		}
		
	}
	
	if (idle_left == 1){
		mat4 temp = mult_matrix(translate(0,0,0), scale(1, 1, 1)); 
		ctm_arr[0] = mult_matrix(ctm_arr[0], rotatex(degree));
		ctm_arr[9] = mult_matrix(ctm_arr[9], rotatex(degree));
		ctm_arr[18] = mult_matrix(ctm_arr[18], rotatex(degree));
		ctm_arr[3] = mult_matrix(ctm_arr[3], rotatex(degree));
		ctm_arr[12] = mult_matrix(ctm_arr[12], rotatex(degree));
		ctm_arr[21] = mult_matrix(ctm_arr[21], rotatex(degree));
		ctm_arr[6] = mult_matrix(ctm_arr[6], rotatex(degree));
		ctm_arr[15] = mult_matrix(ctm_arr[15], rotatex(degree));
		ctm_arr[24] = mult_matrix(ctm_arr[24], rotatex(degree));
		
		swap_ctm[0] = ctm_arr[0];
		swap_ctm[9] = ctm_arr[9];
		swap_ctm[18] = ctm_arr[18];
		swap_ctm[3] = ctm_arr[3];
		swap_ctm[12] = ctm_arr[12];
		swap_ctm[21] = ctm_arr[21];
		swap_ctm[6] = ctm_arr[6];
		swap_ctm[15] = ctm_arr[15];
		swap_ctm[24] = ctm_arr[24];
		
		l_rotation += degree;
		if (l_rotation > 89.999999){
			
			l_rotation = 0;
			idle_left = 0;
			//swap('l');
		}
		
	}
	
	if (idle_right == 1){
		ctm_arr[2] = mult_matrix(ctm_arr[2], rotatex(degree));
		ctm_arr[5] = mult_matrix(ctm_arr[5], rotatex(degree));
		ctm_arr[8] = mult_matrix(ctm_arr[8], rotatex(degree));
		ctm_arr[11] = mult_matrix(ctm_arr[11], rotatex(degree));
		ctm_arr[14] = mult_matrix(ctm_arr[14], rotatex(degree));
		ctm_arr[17] = mult_matrix(ctm_arr[17], rotatex(degree));
		ctm_arr[20] = mult_matrix(ctm_arr[20], rotatex(degree));
		ctm_arr[23] = mult_matrix(ctm_arr[23], rotatex(degree));
		ctm_arr[26] = mult_matrix(ctm_arr[26], rotatex(degree));
		
		swap_ctm[2] = ctm_arr[2];
		swap_ctm[5] = ctm_arr[5];
		swap_ctm[8] = ctm_arr[8];
		swap_ctm[11] = ctm_arr[11];
		swap_ctm[14] = ctm_arr[14];
		swap_ctm[17] = ctm_arr[17];
		swap_ctm[20] = ctm_arr[20];
		swap_ctm[23] = ctm_arr[23];
		swap_ctm[26] = ctm_arr[26];
		
		r_rotation += degree;
		if (r_rotation > 89.999999){
			
			r_rotation = 0;
			idle_right = 0;
			//swap('r');
		}
		
	}
	
	if (idle_up == 1){
		ctm_arr[0] = mult_matrix(ctm_arr[0], rotatey(degree));
		ctm_arr[1] = mult_matrix(ctm_arr[1], rotatey(degree));
		ctm_arr[2] = mult_matrix(ctm_arr[2], rotatey(degree));
		
		ctm_arr[9] = mult_matrix(ctm_arr[9], rotatey(degree));
		ctm_arr[10] = mult_matrix(ctm_arr[10], rotatey(degree));
		ctm_arr[11] = mult_matrix(ctm_arr[11], rotatey(degree));
		
		ctm_arr[18] = mult_matrix(ctm_arr[18], rotatey(degree));
		ctm_arr[19] = mult_matrix(ctm_arr[19], rotatey(degree));
		ctm_arr[20] = mult_matrix(ctm_arr[20], rotatey(degree));
		
		swap_ctm[0] = ctm_arr[0];
		swap_ctm[1] = ctm_arr[1];
		swap_ctm[2] = ctm_arr[2];
		swap_ctm[9] = ctm_arr[9];
		swap_ctm[10] = ctm_arr[10];
		swap_ctm[11] = ctm_arr[11];
		swap_ctm[18] = ctm_arr[18];
		swap_ctm[19] = ctm_arr[19];
		swap_ctm[20] = ctm_arr[20];
		
		u_rotation += degree;
		if (u_rotation > 89.999999){
			u_rotation = 0;
			idle_up = 0;
			//swap('u');
		}
		
	}
	
	if (idle_down == 1){
		ctm_arr[6] = mult_matrix(ctm_arr[6], rotatey(degree));
		ctm_arr[7] = mult_matrix(ctm_arr[7], rotatey(degree));
		ctm_arr[8] = mult_matrix(ctm_arr[8], rotatey(degree));
		
		ctm_arr[15] = mult_matrix(ctm_arr[15], rotatey(degree));
		ctm_arr[16] = mult_matrix(ctm_arr[16], rotatey(degree));
		ctm_arr[17] = mult_matrix(ctm_arr[17], rotatey(degree));
		
		ctm_arr[24] = mult_matrix(ctm_arr[24], rotatey(degree));
		ctm_arr[25] = mult_matrix(ctm_arr[25], rotatey(degree));
		ctm_arr[26] = mult_matrix(ctm_arr[26], rotatey(degree));
		
		swap_ctm[6] = ctm_arr[6];
		swap_ctm[7] = ctm_arr[7];
		swap_ctm[8] = ctm_arr[8];
		swap_ctm[15] = ctm_arr[15];
		swap_ctm[16] = ctm_arr[16];
		swap_ctm[17] = ctm_arr[17];
		swap_ctm[24] = ctm_arr[24];
		swap_ctm[25] = ctm_arr[25];
		swap_ctm[26] = ctm_arr[26];
		
		d_rotation += degree;
		if (d_rotation > 89.999999){
			d_rotation = 0;
			idle_down = 0;
			
			swap('d');
		}
		
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);
	
	if(key == 'f'){
		//rotate front
		idle_front = 1;
		//swap('f');
		
	}
	if (key == 'r'){
		//rotate right
		idle_right = 1;
	}
	if (key == 'b'){
		//rotate back
		idle_back = 1;
	}
	if (key == 'l'){
		//rotate left
		idle_left = 1;
	}
	if (key == 'u'){
		//rotate up
		idle_up = 1;
	}
	if (key == 'd'){
		//rotate down
		idle_down = 1;
	}
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Rubiks");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
