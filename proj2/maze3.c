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
char nicer[height][width];
int arr[4] = {1, 2, 3, 4};
int *dirs = arr;
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

vec4 color() {
    //generates random RGB values for coloring 
    vec4 color;
    color.x = (float) rand() / (float) (RAND_MAX);
    color.y = (float) rand() / (float) (RAND_MAX);
    color.z = (float) rand() / (float) (RAND_MAX);
    color.w = 1.0;

    return color;
}

//vertex and color arrays' placeholder
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
                nicer[i][j] = '.';
                posts++;
                printf("%c ", nicer[i][j]);
            }
            if (maze[i][j] == 2) {
                nicer[i][j] = '.';
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

void divide(int leftbound, int rightbound, int upbound, int lowbound) {
    srand(time(0));
    int r = 1;
    int c = 1;
    int i;
    int upwall = 0;
    int rightwall = 0;
    int downwall = 0;
    int leftwall = 0;

    //Base cases
    if (leftbound + 2 >= rightbound) {
        printf("failed: leftbound %d + 2 >= rightbound %d\n", leftbound, rightbound);
        return;
    }
    if (rightbound - 2 <= leftbound) {
        printf("failed: rightbound %d - 2 >= leftbound %d\n", rightbound, leftbound);
        return;
    }
    if (upbound + 2 >= lowbound) {
        printf("failed: upbound %d + 2 >= lowbound %d\n", upbound, lowbound);
        return;
    }
    if (lowbound - 2 <= upbound) {
        printf("failed: lowbound %d - 2 >= upbound %d\n", lowbound, upbound);
        return;
    }

    while (c % 2 != 0) { //not even number. get random
        c = rand() % ((rightbound - 2) - (leftbound + 2) + 1) + leftbound + 2; //Get you cs from 2 - 14
    }
    //Choose r from 0-16
    while (r % 2 != 0) {
        r = rand() % ((lowbound - 2) - (upbound + 2) + 1) + upbound + 2; //Get you rs from 2 - 14
    }

    maze[r][c];

    //draw wall right
    i = c;
    while (i < rightbound) {
        if (i % 2 == 0) {
            maze[r][i] = 1;
        } else {
            maze[r][i] = 2;
            rightwall++;
        }
        i++;
    }

    //draw wall up
    int j = r;
    while (j > upbound) {
        if (j % 2 == 0) {
            maze[j][c] = 1;
        } else {
            maze[j][c] = 2;
            upwall++;
        }
        j--;
    }

    //draw wall left
    int k = c;
    while (k > leftbound) {
        if (k % 2 == 0) {
            maze[r][k] = 1;
        } else {
            maze[r][k] = 2;
            leftwall++;
        }
        k--;
    }

    //draw wall down
    int m = r;
    while (m < lowbound) {
        if (m % 2 == 0) {
            maze[m][c] = 1;
        } else {
            maze[m][c] = 2;
            downwall++;
        }
        m++;
    }

    int wall;
    int pos;
    int r_knock;
    int d_knock;
    int u_knock;
    int l_knock;

    //choose 1 position on any given wall to keep
    //knock out portions of the other three
    //either 1, 2, 3, or 4. will be different everytime
    pos = (rand() % 5);
    printf("%d ", dirs[pos]);
    switch (dirs[pos]) {
        case 1: //knockout part of right wall
            if (upwall != 0) {
                u_knock = rand() % upwall + 1;
                u_knock = u_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r - u_knock][c] = 0;
            }

            if (downwall != 0) {
                d_knock = rand() % downwall + 1;
                d_knock = d_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r + d_knock][c] = 0;
            }

            if (leftwall != 0) {
                l_knock = rand() % leftwall + 1;
                l_knock = l_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r][c - l_knock] = 0;
            }
            break;
        case 2: //knockout part of up wall
            if (upwall != 0) {
                u_knock = rand() % upwall + 1;
                u_knock = u_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r - u_knock][c] = 0;
            }

            if (downwall != 0) {
                d_knock = rand() % downwall + 1;
                d_knock = d_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r + d_knock][c] = 0;
            }

            if (leftwall != 0) {
                l_knock = rand() % leftwall + 1;
                l_knock = l_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r][c - l_knock] = 0;
            }
            break;
        case 3: //keep left wall
            if (upwall != 0) {
                u_knock = rand() % upwall + 1;
                u_knock = u_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r - u_knock][c] = 0;
            }

            if (downwall != 0) {
                d_knock = rand() % downwall + 1;
                d_knock = d_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r + d_knock][c] = 0;
            }

            if (rightwall != 0) {
                r_knock = rand() % rightwall + 1;
                r_knock = r_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r][c + r_knock] = 0;
            }
            break;
        case 4: //keep down wall
            if (upwall != 0) {
                u_knock = rand() % upwall + 1;
                u_knock = u_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r - u_knock][c] = 0;
            }

            if (rightwall != 0) {
                r_knock = rand() % rightwall + 1;
                r_knock = r_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r][c + r_knock] = 0;
            }

            if (leftwall != 0) {
                l_knock = rand() % leftwall + 1;
                l_knock = l_knock * 2 - 1;
                printf("Set to 0\n");
                maze[r][c - l_knock] = 0;
            }
            break;
    }


	printNicer(nicer);

	divide(leftbound, upbound, c, r);
	divide(c, upbound, rightbound, r);
	divide(leftbound, r, c, lowbound);
	divide(c, r, rightbound, lowbound);

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
    printMaze(maze);

    //choose starting/ending points of the maze
    //generate random number to determine which portion of the walls 
    //will get knocked out
    srand(time(0));
    int entrance = rand() % height;
    int exit = rand() % height;
    printf("entrance: %d\n", entrance);
    printf("exit: %d\n", exit);
    if (entrance % 2 == 0) {
        if (entrance == height - 1) {
            entrance -= 1;
            printf("entrance after correction1: %d\n", entrance);
        } else {
            entrance += 1;
            printf("entrance after correction2: %d\n", entrance);
        }
    }
    if (exit % 2 == 0) {
        if (exit == height - 1) {
            exit -= 1;
            printf("exit after correction1: %d\n", exit);
        } else {
            exit += 1;
            printf("exit after correction2: %d\n", exit);
        }
    }

    //set the positions to 0, signifying they're now open
    maze[entrance][0] = 0;
    maze[exit][width - 1] = 0;
    printMaze(maze);

    divide(0, width - 1, 0, height - 1);

    printMaze(maze);
    printf("the maze contains %d walls and %d posts\n", walls, posts);
}

/*Dr. Tan's code*/

void init(void) {
    generate(maze);

    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (vertices) + sizeof (colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof (vertices), sizeof (colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof (vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) & ctm);
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
