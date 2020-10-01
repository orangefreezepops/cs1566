/*
 * Keegan Fouse
 * kdf11@pitt.edu 
 * 
 * cone.c
 *
 *  Adopted on: Sept. 22, 2019
 *      From Thumrongsak Kosiyatrakul's triangle.c
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

double rad = (M_PI / 180);
vec4 vertices[360];
vec4 colors[360];
int num_vertices = 360; //(360 / 6degrees) * 3vertices * 2 (base and cone)

GLuint ctm_location;
mat4 ctm;
GLfloat x_value = 0.0;
int isGoingRight = 1;

vec4 color(){
	vec4 color;
	color.x = (float)rand() / (float)(RAND_MAX);
	color.y = (float)rand() / (float)(RAND_MAX);
	color.z = (float)rand() / (float)(RAND_MAX);
	color.w = 1.0;
	
	return color;
}

void cone(){
	
	/*specs for the triangles in the base -- start at origin*/
	vec4 origin = {0.0, 0.0, 0.0, 1.0};
	vec4 right = {0.0, 0.0, 0.0, 1.0};
	vec4 left = {0.0, 0.0, 0.0, 1.0};
	
	int v_index = 0;
	
	for (double angle = 0; angle <= 360; angle += 6){
		left.x = right.x;
		left.z = right.z;
		right.x = cos(angle * rad);
		right.z = sin(angle * rad);
		
		if (angle != 0){
			vec4 curr = color();
			
			colors[v_index] = curr;
			vertices[v_index++] = origin;
			colors[v_index] = curr;
			vertices[v_index++] = left;
			colors[v_index] = curr;
			vertices[v_index++] = right;
		}
	}
	
	/*cone generation*/
	vec4 top = {0.0, 1.0, 0.0, 1.0}; //changes to y=1 so its standing up
	//these stay the same, but reset them
	vec4 t_right = {0.0, 0.0, 0.0, 1.0};
	vec4 t_left = {0.0, 0.0, 0.0, 1.0};
	
	for (double angle = 0; angle <= 360; angle += 6){
		t_left.x = t_right.x;
		t_left.z = t_right.z;
		t_right.x = cos(angle * rad);
		t_right.z = sin(angle * rad);
		
		if (angle != 0){
			vec4 curr = color();
			colors[v_index] = curr;
			vertices[v_index++] = top;
			colors[v_index] = curr;
			vertices[v_index++] = t_left;
			colors[v_index] = curr;
			vertices[v_index++] = t_right;		
		}
	}
}

/*Dr. Tan's code*/

void init(void)
{
	cone();
    GLuint program = initShader("vshader_ctm.glsl", "fshader_ctm.glsl");
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
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

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

}

void idle(void){
	
	if(isGoingRight){
		x_value += 0.5;
		if(fabs(x_value - 1.0) < 0.000001){
			x_value = 1.0;
			isGoingRight = 0;
		}
	} else {
		x_value -= 0.5;
		if (fabs(x_value - (-1.0)) < 0.000001){
			x_value = -1.0;
			isGoingRight = 1;
		}
	}
	ctm = translate(x_value, 0.0, 0.0);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Cone with CTM");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
