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
vec4 vertices[24576];
vec4 colors[24576];
int num_vertices = 24576; //(8 sides of octohedron) * (1024 triangles per side) * (3 vertices) = 24576

int maze[height][width];
int posts = 0;
int walls = 0;

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

int * directions(){
	int *arr = malloc(sizeof(int));
	int i = 0;
	//printf("directions array: ");
	for (i = 0; i < 4; i ++){
		arr[i] = i + 1;
		//printf("%d ", arr[i]);
	}
	return arr;
}

void printMaze(int maze[height][width]){
	int i = 0;
	int j = 0;
	for (i = 0; i < height; i ++){
		for (j = 0; j < width; j ++){
		printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int * shuffle(int *arr, int size){
	int i = 0;
	srand(time(0));
	if (size > 1){
		for (i = 0; i < size - 1; i ++){
			int j = i + rand() / (RAND_MAX / (size - 1) + 1);
			int t = arr[j];
			arr[j] = arr[i];
			arr[i] = t;
		}
	}
	/* for (i = 0; i < size; i ++){
		printf("%d ", arr[i]);
	} */
	return arr;
}

void divide(int r, int c, int *dirs){
	//generate random directions
	dirs = shuffle(dirs, 4);
	int i = 0;
	//start algorithm
	for (i = 0; i < sizeof(dirs); i ++){
		//1 = up, 2 = right, 3 = down, 4 = left
		if (c == 0 || c == width) {
			switch(dirs[i]){
				case 1: //up
					//should never go up on the boundary
					break;
			case 2: //right
				if (c + 2 >= width - 1){
					continue;
				}
				if (maze[r][c + 2] != 0 && (maze[r][c + 2] != 1)){
					maze[r][c + 2] = 0;
					maze[r][c + 1] = 0;
					divide(r, c + 2, dirs);
				}
				break;
			case 3: //down
				//should never go down on the boundary
				break;
			case 4: //left
				if (c - 2 <= 0){
					continue;
				}
				if (maze[r][c - 2] != 0 && (maze[r][c - 2] != 1 && maze[r][c - 1] != 1)){
					maze[r][c - 2] = 0;
					maze[r][c - 1] = 0;
					divide(r, c - 2, dirs);
				}
				break;
			}
		}
		if (r == 0 || r == height){
			switch(dirs[i]){
				case 1: //up
					//should never go up on the boundary
					break;
			case 2: //right
				//should never go right on the boundary
				break;
			case 3: //down
				if (r - 2 <= 0){
					continue;
				}
				if (maze[r - 2][c] != 0 && (maze[r-2][c] != 1 && maze[r-1][c] != 1)){
					maze[r-2][c] = 0;
					maze[r-1][c] = 0;
					divide(r-2, c, dirs);
				}
				break;
			case 4: //left
				//should never go left on the boundary
				break;
			}
		}
		//if its not in the first row or last row
		else {
			switch(dirs[i]){
				case 1: //up
					if ((r - 2 <= 0)){
						continue;
					}	
					if ((maze[r - 2][c] != 0) && (maze[r - 2][c] != 1 && maze[r - 1][c] != 1)){
						maze[r - 2][c] = 0;
						maze[r - 1][c] = 0;
						divide(r - 2, c, dirs);
					}
					break;
				case 2: //right
					if (c + 2 >= width - 1){
						continue;
					}
					if ((maze[r][c + 2] != 0) && (maze[r][c + 2] != 1 && maze[r][c + 1] != 1)){
						maze[r][c + 2] = 0;
						maze[r][c + 1] = 0;
						divide(r, c + 2, dirs);
					}
					break;
				case 3: //down
					if (r + 2 >= height - 1){
						continue;
					}
					if ((maze[r + 2][c] != 0) && (maze[r + 2][c] != 1 && maze[r + 1][c] != 1)){
						maze[r + 2][c] = 0;
						maze[r + 2][c] = 0;
						divide(r + 2, c, dirs);
					}
					break;
				case 4: //left
					if ((c - 2 <= 0)){
						continue;
					}
					if ((maze[r][c - 2] != 0) && (maze[r][c - 2] != 1 && maze[r][c - 1] != 1)){
						maze[r][c - 2] = 0;
						maze[r][c - 2] = 0;
						divide(r, c - 2, dirs);
					}
					break;
			}
		}
	}
}

void generate(){
	//initialize maze
	int i = 0;
	int j = 0;
	for (i = 0; i < height; i ++){
		for (j = 0; j < width; j ++){
			if (i %2 == 0){
				if (j%2 == 0){
					maze[i][j] = 1;
				} else {
					maze[i][j] = 2;
				}
			}
			else if (i%2 != 0){
				if (j%2 == 0){
					maze[i][j] = 2;
				} else {
					maze[i][j] = 0;
				}
			}
		}
	}
	//initial maze
	printMaze(maze);
	
	//choose starting/ending points of the maze
	//generate random number to determine which portion of the walls 
	//will get knocked out
	srand(time(0));
	int entrance = rand() % height;
	int exit = rand() % height;
	printf("entrance: %d\n", entrance);
	printf("exit: %d\n", exit);
	if (entrance%2 == 0){
		if (entrance == height - 1){
			entrance -= 1;
			printf("entrance after correction1: %d\n", entrance);
		} else {
			entrance += 1;
			printf("entrance after correction2: %d\n", entrance);
		}
	}
	if (exit%2 == 0){
		if (exit == height - 1){
			exit -= 1;
			printf("exit after correction1: %d\n", exit);
		} else {
			exit += 1;
			printf("exit after correction2: %d\n", exit);
		}
	}
	
	//set the positions to 0, signifying they're now open
	maze[entrance][0] = 0;
	maze[exit][width-1] = 0;
	printMaze(maze);
	
	int *rand_dirs = directions();
	
	int r = entrance;
	int c = 0;
	divide(r, c, rand_dirs);
	
	printMaze(maze);
	char nicer[height][width];
	
	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			if(maze[i][j] == 1){
				nicer[i][j] = '.';
				posts ++;
				printf("%c ", nicer[i][j]);
			}
			if(maze[i][j] == 2){
				nicer[i][j] = '.';
				walls++;
				printf("%c ", nicer[i][j]);
			}
			if (maze[i][j] == 0){
				nicer[i][j] = ' ';
				printf("%c ", nicer[i][j]);
			}
		}
		printf("\n");
	}
	printf("the maze contains %d walls and %d posts\n", walls, posts);
}

/*void maze(){
	/*make sample grid
	int row = 8;
	int column = 8;
	char maze[row][column];
	int i, j = 0;
	printf("start: \n");
	printf(" _ _ _ _ _ _ _ _\n");
	for (i = 0; i < row; i++){
		for (j = 0; j < column; j++){
			maze[i][j] = '|';
			
			if(j%2 == 0 && j == 0){
				printf("|_");
				printf("%c", maze[i][j]);
			} else {
				printf("_");
				printf("%c", maze[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
	
	/*build maze/
	generate(maze);
}*/


/*Dr. Tan's code*/

void init(void)
{
	generate(maze);
	
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

void idle(void){
	glutPostRedisplay();
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
	glEnable(GL_DEPTH_TEST);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("maze");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
