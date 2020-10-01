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

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

double rad = (M_PI / 180);
vec4 vertices[16200];
vec2 tex_coords[16200];
int num_vertices = 16200; //36 per "cube" structure... (81 columns + 79 walls)*36 + (19*19 floor tiles)*36
int v_index = 0;
int maze[17][17];
char nicer[17][17];
int posts = 0;
int walls = 0;
int solved = 0;
int is_idle = 1;

int sx = 0;
int sy = 0;
int sz = 0;
mat4 frusty;

enum dir {
	north,
	east,
	south,
	west
};

vec4 from = {0, 0, 0, 1};


double degree = 0;

GLuint model_view_location;
mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint projection_location;
mat4 projection_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

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


// Colors (not really use this but send it into the pipeline anyway)

vec4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},  // red
 {1.0, 0.0, 0.0, 1.0},  // red
 {1.0, 0.0, 0.0, 1.0},  // red
 {0.0, 1.0, 0.0, 1.0},  // green
 {0.0, 1.0, 0.0, 1.0},  // green
 {0.0, 1.0, 0.0, 1.0}}; // green


vec2 brick[36] = {
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
	{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5}};
	
vec2 stone[36] = {
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
	{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5}};
	
vec2 grass[36] = {
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
	{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0}};
	
void printNicer(char nicer[17][17]) {
    int i, j;
    walls = 0;
    posts = 0;
    for (i = 0; i < 17; i++) {
        for (j = 0; j < 17; j++) {
            if (maze[i][j] == 1) {
                nicer[i][j] = '+';
                posts++;
                printf("%c ", nicer[i][j]);
            }
            if (maze[i][j] == 2) {
                nicer[i][j] = '-';
                walls++;
                printf("%c ", nicer[i][j]);
            }
            if (maze[i][j] == 0) {
                nicer[i][j] = ' ';
                printf("%c ", nicer[i][j]);
            }
        }
        printf("\n");
    }
}
void facing_up(){
	
	
	
	
	
	
}

void solve_maze(int curr_row, int curr_col, int fin_row, int fin_col){
	while (!solved){	
			while (maze[curr_row][curr_col] == 0){
			
				if (curr_row == fin_row && curr_col == fin_col){
					solved = 1;
				}
				//try going left
				else if (maze[curr_row-1][curr_col] == 0){
					curr_row--;
					solved = 0;
				}
				//try going 
				else if (maze[curr_row][curr_col+1] == 0){
					curr_col++;
					solved = 0;
				}
				else if (maze[curr_row++][curr_col] == 0){
					curr_row++;
					solved = 0;
				}
				else if (maze[curr_row][curr_col-1] == 0){
					curr_col--;
					solved = 0;
				}
			}
			
			//check right
			
			//check right
		}
	}
	

void build_maze(){
	//initialize maze
    int i = 0;
    int j = 0;
    for (i = 0; i < 17; i++) {
        for (j = 0; j < 17; j++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    maze[i][j] = 1;
                } else {
                    maze[i][j] = 2;
                }
            } else if (i % 2 != 0) {
                if (j % 2 == 0) {
                    maze[i][j] = 2;
                } else {
                    maze[i][j] = 0;
                }
            }
        }
    }
    //initial maze
    //printMaze(maze);
	int entrance = 3;
	int exit = 13;
	
	maze[entrance][0] = 0;
	maze[exit][17 - 1] = 0;
	
	//build static maze
	for (i = 1; i < 17-1; i ++){
		maze[i][1] = 0;
		maze[15][i] = 0;
		maze[14][13] = 0;
	}
	
	for (i = 13; i > 2; i --){
		maze[13][i] = 0;
	}
	
	for(i = 13; i > 1; i --){
		maze[i][3] = 0;
	}
	
	for (i = 3; i < 16; i ++){
		maze[1][i] = 0;
	}
	
	for (i = 15; i > 4; i --){
		maze[2][15] = 0;
		maze[3][i] = 0;
	}
	maze[4][5] = 0;
	
	for (i = 5; i < 15; i ++){
		maze[5][i] = 0;
	}
	
	for (i = 5; i < 16; i ++){
		maze[11][i] = 0;
	}
	maze[8][5] = 0;
	maze[10][5] = 0;
	maze[9][6] = 0;
	maze[8][7] = 0;
	maze[7][8] = 0;
	maze[8][9] = 0;
	maze[9][10] = 0;
	maze[8][11] = 0;
	maze[7][12] = 0;
	maze[8][13] = 0;
	maze[9][14] = 0;
	maze[6][15] = 0;
	maze[7][15] = 0;
	maze[8][15] = 0;
	maze[12][15] = 0;
	
	//divide(0, 16, 0, 16);
	
	printNicer(nicer);
    printf("the maze contains %d walls and %d posts\n", walls, posts);
	
	enum dir facing = east;
	//solve_maze(entrance, 0, exit, 17, facing);
}


void fill_maze(){
	v_index = 0;
	int i, j, k;
	float posx = 0;
	float posz = 0;
	mat4 temp, floor;
	for(i = 0; i < 17; i ++){
		posx = 0;
		for (j = 0; j < 17; j ++){
		
			if (maze[i][j] == 1){
				//posts
				temp = mult_matrix(translate(posx-8, 0, posz-8), scale(0.5, 2.2, 0.5));
				for(k = 0; k < 36; k ++){
					vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
					tex_coords[v_index] = stone[k];
					v_index++;
				}
			}
			if (maze[i][j] == 2){
				if (j % 2 == 0) {
					//horizontal walls
					temp = mult_matrix(translate(posx-8, 0, posz-8), scale(0.3, 2, 1.5));
					for(k = 0; k < 36; k ++){
						vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
						tex_coords[v_index] = brick[k];
						v_index++;
					}
				} else {
				//vertical walls
					temp = mult_matrix(translate(posx-8, 0, posz-8), scale(1.5, 2, 0.3));
					for(k = 0; k < 36; k ++){
						vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
						tex_coords[v_index] = brick[k];
						v_index++;
					}
				}
			} 
			posx ++;
		}
		posz ++;
	}
}

void fill_floor(){
	int i, j, k, posx, posz = 0;
	mat4 temp;
	for (i = 0; i < 17; i ++){
		posx = 0;
		for (j = 0; j < 17; j ++){
			temp = mult_matrix(translate(posx-8, 0, posz-8), scale(3, 0.005, 3));
			for (k = 0; k < 36; k++){
				vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
				tex_coords[v_index] = grass[k];
				v_index++;
			}
			posx++;
		}
		posz++;
	}
}


void init(void)
{
    int width = 800;
    int height = 800;
    GLubyte my_texels[width][height][3];

    FILE *fp = fopen("p2texture04.raw", "r");
    fread(my_texels, width * height * 3, 1, fp);
    fclose(fp);
	
	build_maze();
	fill_maze();
	fill_floor();
	

	GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

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
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));

    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    glEnable(GL_CULL_FACE);
	model_view_location = glGetUniformLocation(program, "model_view_matrix");
	projection_location = glGetUniformLocation(program, "projection_matrix");
	
	
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
	
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glutSwapBuffers();
}

void start_maze(){
	vec4 eye = {-10, 0.5, -5, 1};
	vec4 at = {0.5, 0, -5, 1.0};
	vec4 up = {0, 1, 0, 1.0};
	model_view_matrix = mult_matrix(frusty, look_at(from, at, up));
}
int count = 0;

void keyboard(unsigned char key, int mousex, int mousey)
{
	frusty = frustum(-.5, .5, -.5, .5, -.5, .5);
    if(key == 'q')
    	exit(0);

	//start maze
	if (key == 's'){
		is_idle = 2;
	}
	
	if(key == 'c'){
		printf("count: %d", count);
		is_idle = 0;
		
	}

    glutPostRedisplay();
}



void idle(){
	//do nuffin
	if (is_idle == 0){
	
	}
	//circle around
	if (is_idle == 1){
		degree += 0.001;
		count ++;
		frusty = frustum(-.5, .5, -.5, .5, -.5, .5);
		vec4 eye = {15 * cos(degree), 10, 15 * sin(degree), 1.0};
		vec4 at = {0, 0, 0, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		from = eye;
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
		if (count == 3230){
			is_idle = 2;
		}
	} 
	//fly down
	if (is_idle == 2){
		//vec_print(from);
		from.y -= 0.01;
		from.x += 0.0055;
		from.z -= 0.0035;
		count++;
		vec4 at = {0.5, 0, -5, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		model_view_matrix = mult_matrix(frusty, look_at(from, at, up));
		if (count == 4175){
			is_idle = 3;
		}
	}
	//solve eventually
	if (is_idle == 3){
		vec4 eye = {-9.5, 0.5, -5, 1.0};
		vec4 at = {0.5, 0, -5, 1.0};
		vec4 up = {0, 1, 0, 1.0};
		model_view_matrix = mult_matrix(frusty, look_at(eye, at, up));
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Maze");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
