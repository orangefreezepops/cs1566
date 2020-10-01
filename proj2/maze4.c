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

#define bool int

#define false 0
#define true 1
#define height 17
#define width 17

/*my methods*/

double rad = (M_PI / 180);
vec4 vertices[5796];
vec4 colors[5796];
vec4 wall_vertices[2844];
vec4 post_vertices[2916];
vec2 tex_coords[5796];
int num_vertices = 5796; //36 per "cube" structure... (81 columns + 79 walls + 1) * 36

int maze[height][width];
char nicer[height][width];
int posts = 0;
int walls = 0;

GLuint model_view_location;
mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint projection_location;
mat4 projection_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

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

vec2 wall_tex_coords[36] = {
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5},
		{0.0, 0.0}, {0.5, 0.0}, {0.5, 0.5}, {0.0, 0.0}, {0.5, 0.5}, {0.0, 0.5}};
		
vec2 floor_tex_coords[36] = {
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0},
		{0.0, 0.5}, {0.5, 0.5}, {0.5, 1.0}, {0.0, 0.5}, {0.5, 1.0}, {0.0, 1.0}};

vec2 post_tex_coords[36] = {
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5},
		{0.5, 0.0}, {1.0, 0.0}, {1.0, 0.5}, {0.5, 0.0}, {1.0, 0.5}, {0.5, 0.5}};

//vertex and cor arrays' placeholder
int v_index = 0;

void printMaze(int maze[height][width]) {
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printNicer(char nicer[height][width]) {
    int i, j;
    walls = 0;
    posts = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
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

void create_h_wall(int row, int col){
	
	int move_z;
	int move_x;
	int i;
	
	if (row < 8){
		move_z = (row/8) - 1;
	} else {
		move_z = (row - 8)/8;
	}
	
	if (col < 8){
		move_x = (col/8) - 1;
	} else {
		move_x = (col - 8)/8;
	}
	
	mat4 temp = mult_matrix(translate(move_x, 0, move_z), scale(0.7, 0.5, 0.5));
	
	for (i = 0; i < 36; i ++){
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[i]);
		tex_coords[v_index] = wall_tex_coords[i];
		v_index++;
	}
}

void create_v_wall(int row, int col){
	
	int move_z;
	int move_x;
	int i;
	
	if (row < 8){
		move_z = (row/8) - 1;
	} else {
		move_z = (row - 8)/8;
	}
	
	if (col < 8){
		move_x = (col/8) - 1;
	} else {
		move_x = (col - 8)/8;
	}
	
	mat4 temp = mult_matrix(translate(move_x, 0, move_z), scale(0.5, 0.5, 0.7));
	
	for (i = 0; i < 36; i ++){
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[i]);
		tex_coords[v_index] = wall_tex_coords[i];
		v_index++;
	}
}

void create_post(int row, int col){
	int i;
	int move_z;
	int move_x;
	
	if (row < 8){
		move_z = (row/8) - 1;
	} else {
		move_z = (row - 8)/8;
	}
	
	if (col < 8){
		move_x = (col/8) - 1;
	} else {
		move_x = (col - 8)/8;
	}
	
	mat4 temp = mult_matrix(translate(move_x, 0, move_z), scale(0.3, 0.8, 0.3));
	
	for (i = 0; i < 36; i ++){
		vertices[v_index] = matrix_x_vector(temp, cube_vertices[i]);
		tex_coords[v_index] = post_tex_coords[i];
		v_index++;
	}
}
void generate() {
    //initialize maze
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
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
	maze[exit][width - 1] = 0;
	
	//build static maze
	for (i = 1; i < height-1; i ++){
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
	
	printNicer(nicer);
	v_index=0;
	
	for(i = 0; i < 16; i ++){
		for(j = 0; j < 16; j++){
			if (maze[i][j] == 1){
				printf("building posts\n");
				create_post(i, j);
			}
			if (maze[i][j] == 2){
				if (j % 2 == 0){
					//build horizontal wall
					printf("building horizontal wall\n");
					create_h_wall(i,j);
				} else {
					//build vertical wall
					printf("building vertical wall\n");
					create_v_wall(i,j);
				}
			}
		}
	}
	
	
    //printMaze(maze);
    printf("the maze contains %d walls and %d posts\n", walls, posts);
}

/*Dr. Tan's code*/

void init(void) {
	generate();
	
	int w = 800;
    int h = 800;
    GLubyte my_texels[w][h][3];
	
	FILE *fp = fopen("p2texture04.raw", "r");
    fread(my_texels, w * h * 3, 1, fp);
    fclose(fp);

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

    printf("texture_location: %i\n", texture_location);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glDepthRange(1,0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    glutSwapBuffers();
}

void idle(void) {
	
    glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey) {
    if (key == 'q')
        exit(0);

    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("maze");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
