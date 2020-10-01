/*
 * Keegan Fouse
 * kdf11@pitt.edu 
 * 
 * cubes.c
 *
 *  Adapted on: Oct. 31, 2019
 *      From Thumrongsak Kosiyatrakul's
 *		and My cube.c 
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


/*my methods*/

typedef struct
{
    GLfloat x;
    GLfloat y;
} vec2;

double rad = (M_PI / 180);
vec4 vertices[108];
vec4 colors[108];
int num_vertices = 108; 
int v_index = 0;

vec2 tex_coords[36] = {
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5}};

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
	
vec4 cube_colors[36] = 
{
	//cube front
	{0.0, 1.0, 0.0, 1.0},	// bottom left
    {0.0, 1.0, 0.0, 1.0},	// bottom right
    {0.0, 1.0, 0.0, 1.0},	// top right
    {0.0, 1.0, 0.0, 1.0},	// bottom left
    {0.0, 1.0, 0.0, 1.0},	// top right
    {0.0, 1.0, 0.0, 1.0},   // top left
	
	//cube right
	{0.0, 1.0, 1.0, 1.0},	// bottom left
    {0.0, 1.0, 1.0, 1.0},	// bottom right
    {0.0, 1.0, 1.0, 1.0},	// top right
    {0.0, 1.0, 1.0, 1.0},	// bottom left
    {0.0, 1.0, 1.0, 1.0},	// top right
    {0.0, 1.0, 1.0, 1.0},   // top left
	
	//cube back
	{0.5, 0.5, 0.5, 1.0},	// bottom left
    {0.5, 0.5, 0.5, 1.0},	// bottom right
    {0.5, 0.5, 0.5, 1.0},	// top right
    {0.5, 0.5, 0.5, 1.0},	// bottom left
    {0.5, 0.5, 0.5, 1.0},	// top right
    {0.5, 0.5, 0.5, 1.0},   // top left
	
	//cube left
	{1.0, 0.0, 1.0, 1.0},	// bottom left
    {1.0, 0.0, 1.0, 1.0},	// bottom right
    {1.0, 0.0, 1.0, 1.0},	// top right
    {1.0, 0.0, 1.0, 1.0},	// bottom left
    {1.0, 0.0, 1.0, 1.0},	// top right
    {1.0, 0.0, 1.0, 1.0},   // top left
	
	//cube top
	{1.0, 0.0, 0.0, 1.0},	// bottom left
    {1.0, 0.0, 0.0, 1.0},	// bottom right
    {1.0, 0.0, 0.0, 1.0},	// top right
    {1.0, 0.0, 0.0, 1.0},	// bottom left
    {1.0, 0.0, 0.0, 1.0},	// top right
    {1.0, 0.0, 0.0, 1.0},   // top left
	
	//cube bottom
	{0.0, 0.0, 1.0, 1.0},	// bottom left
    {0.0, 0.0, 1.0, 1.0},	// bottom right
    {0.0, 0.0, 1.0, 1.0},	// top right
    {0.0, 0.0, 1.0, 1.0},	// bottom left
    {0.0, 0.0, 1.0, 1.0},	// top right
    {0.0, 0.0, 1.0, 1.0}    // top left
	};

GLuint ctm_location;
mat4 ctm;

vec4 twin_cube_location = {0.0, 0.5, 0.0, 1.0};
vec4 left_cube_location = {-0.5, -0.5, 0.0, 1.0};
vec4 right_cube_location = {0.5, -0.5, 0.0, 1.0};

GLfloat twin_cube_degree = 0.0, left_cube_degree = 0.0, right_cube_degree = 0.0;

mat4 twin_cube_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 left_cube_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
mat4 right_cube_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

void cubes(){
	int i = 0;
	mat4 temp_tr1 = mult_matrix(translate(-0.5, 0.5, 0.0), scale(0.5, 0.5, 0.5));
	mat4 temp_tr2 = mult_matrix(translate(0.5, 0.5, 0.0), scale(0.5, 0.5, 0.5));

	for (i = 0; i < 36; i ++){
		vertices[v_index] = matrix_x_vector(temp_tr1, cube_vertices[i]);
		colors[v_index] = cube_colors[i];
		v_index ++;
	}

	for (i = 0; i < 36; i++){
		vertices[v_index] = matrix_x_vector(temp_tr2, cube_vertices[i]);
		colors[v_index] = cube_colors[i];
		v_index ++;
	}

	for (i = 0; i < 36; i ++){
		vertices[v_index] = cube_vertices[i];
		colors[v_index] = cube_colors[i];
		v_index ++;
	}
	
}
/*Dr. Tan's code*/

void init(void)
{
	cubes();
	
	int width = 800;
    int height = 800;
    GLubyte my_texels[width][height][3];

    FILE *fp = fopen("p2texture04.raw", "r");
    fread(my_texels, width * height * 3, 1, fp);
    fclose(fp);
	
    GLuint program = initShader("vshader_ctm.glsl", "fshader_ctm.glsl");
    glUseProgram(program);
	ctm_location = glGetUniformLocation(program, "ctm");

	GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);
	
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
	
	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    printf("texture_location: %i\n", texture_location);

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &twin_cube_ctm);
    glDrawArrays(GL_TRIANGLES, 0, 72);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &left_cube_ctm);
	glDrawArrays(GL_TRIANGLES, 72, 36);
	
	glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &right_cube_ctm);
	glDrawArrays(GL_TRIANGLES, 72, 36);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

}

void idle(void){
	
	mat4 temptwin;
	mat4 tempright;
	mat4 templeft;
	
	temptwin = mult_matrix(rotatey(twin_cube_degree), translate(0, 0, 0));
	twin_cube_ctm = temptwin;
	
	templeft = mult_matrix(translate(-0.5, -0.5, 0.0), scale(0.5, 0.5, 0.5));
	left_cube_ctm = mult_matrix(templeft, rotatez(left_cube_degree));
	
	tempright = mult_matrix(translate(0.5, -0.5, 0.0), scale(0.5, 0.5, 0.5));
	right_cube_ctm = mult_matrix(tempright, rotatex(right_cube_degree));
	
	twin_cube_degree += 0.05;
	left_cube_degree += 0.05;
	right_cube_degree += 0.05;
	
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("cubies :)");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
