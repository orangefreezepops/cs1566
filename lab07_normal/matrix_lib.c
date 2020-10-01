/*
	author: Keegan Fouse
	Matrix Library for CS1566
	Intro to Computer Graphics
*/


#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include "matrix_lib.h"

void vec_print(vec4 v){
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.x, v.y, v.z, v.w);
}

vec4 scalar_vec_mult(vec4 v, float s){

	vec4 result;
	
	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	result.w = v.w * s;
	
	return result;
}

vec4 add_vec(vec4 v1, vec4 v2){
	
	vec4 result;
	
	result.x = (v1.x + v2.x);
	result.y = (v1.y + v2.y);
	result.z = (v1.z + v2.z);
	result.w = (v1.w + v2.w);
	
	return result;
}

/*
	given two vectors v1, v2, subtract vector 2 from vector 1
	return the result vector
*/
vec4 sub_vec(vec4 v1, vec4 v2){
	
	vec4 result;
	
	result.x = (v1.x - v2.x);
	result.y = (v1.y - v2.y);
	result.z = (v1.z - v2.z);
	result.w = (v1.w - v2.w);
	
	return result;
}

/*
	do the dot product operation of two vectors and return the
	resulting scalar 
*/
float dot_product(vec4 v1, vec4 v2){
	
	float result;
	result = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
	return result;
}

/*
	given two vectors v1, v2 calculate the cross product for the first 3
	elements in the vector (x, y, z) and ignore the last (w)
	return the resulting vector with the 4th element equal to 0
*/
vec4 cross_product(vec4 v1, vec4 v2){
	vec4 result;
	
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);
	result.w = 0;
	
	return result;
}

float magnitude(vec4 v){
	float result;
	result = sqrt((pow(v.x, 2)) + (pow(v.y, 2)) + (pow(v.z, 2)) + (pow(v.w, 2)));
	return result;
}

vec4 normalize(vec4 v){
	vec4 result;
	result = scalar_vec_mult(v, (1/magnitude(v)));
	return result;
}


/************************** MATRIX OPERATIONS ********************************/

/*
	print the matrix to the consol for debugging purposes
*/
void matrix_print(mat4 m){
	
	printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.x, m.y.x, m.z.x, m.w.x);
	printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.y, m.y.y, m.z.y, m.w.y);
	printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.z, m.y.z, m.z.z, m.w.z);
	printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.w, m.y.w, m.z.w, m.w.w);	
}

/*
	scalar matrix multiplication, result should be a a matrix 
*/
mat4 scalar_x_matrix(mat4 m, float s){
	mat4 result;
	
	//first row
	result.x.x = s * m.x.x;
	result.y.x = s * m.y.x;
	result.z.x = s * m.z.x;
	result.w.x = s * m.w.x;
	
	//second row
	result.x.y = s * m.x.y;
	result.y.y = s * m.y.y;
	result.z.y = s * m.z.y;
	result.w.y = s * m.w.y;
	
	//third row
	result.x.z = s * m.x.z;
	result.y.z = s * m.y.z;
	result.z.z = s * m.z.z;
	result.w.z = s * m.w.z;
	
	//fourth row
	result.x.w = s * m.x.w;
	result.y.w = s * m.y.w;
	result.z.w = s * m.z.w;
	result.w.w = s * m.w.w;
	
	return result;
}

/*
	given two matrices add them together and return the result matrix
*/
mat4 add_matrix(mat4 m1, mat4 m2){
	
	//row 1
	m1.x.x += m2.x.x;
	m1.y.x += m2.y.x;
	m1.z.x += m2.z.x;
	m1.w.x += m2.w.x;
	
	//row 2
	m1.x.y += m2.x.y;
	m1.y.y += m2.y.y;
	m1.z.y += m2.z.y;
	m1.w.y += m2.w.y;

	//row 3
	m1.x.z += m2.x.z;
	m1.y.z += m2.y.z;
	m1.z.z += m2.z.z;
	m1.w.z += m2.w.z;
	
	//row 4
	m1.x.w += m2.x.w;
	m1.y.w += m2.y.w;
	m1.z.w += m2.z.w;
	m1.w.w += m2.w.w;
	
	return m1;
}

/*
	given two matrices, subtract matrix 2 from matrix 1, return the 
	result
*/
mat4 sub_matrix(mat4 m1, mat4 m2){
	
	//row 1
	m1.x.x -= m2.x.x;
	m1.y.x -= m2.y.x;
	m1.z.x -= m2.z.x;
	m1.w.x -= m2.w.x;
	
	//row 2
	m1.x.y -= m2.x.y;
	m1.y.y -= m2.y.y;
	m1.z.y -= m2.z.y;
	m1.w.y -= m2.w.y;

	//row 3
	m1.x.z -= m2.x.z;
	m1.y.z -= m2.y.z;
	m1.z.z -= m2.z.z;
	m1.w.z -= m2.w.z;
	
	//row 4
	m1.x.w -= m2.x.w;
	m1.y.w -= m2.y.w;
	m1.z.w -= m2.z.w;
	m1.w.w -= m2.w.w;
	
	return m1;
}

/*
	given two matrices multiply them
*/
mat4 mult_matrix(mat4 m1, mat4 m2){
	
	mat4 result;
	
	//row 1
	result.x.x = (m2.x.x * m1.x.x) + (m2.x.y * m1.y.x) + (m2.x.z * m1.z.x) + (m2.x.w * m1.w.x);
	result.y.x = (m2.y.x * m1.x.x) + (m2.y.y * m1.y.x) + (m2.y.z * m1.z.x) + (m2.y.w * m1.w.x);
	result.z.x = (m2.z.x * m1.x.x) + (m2.z.y * m1.y.x) + (m2.z.z * m1.z.x) + (m2.z.w * m1.w.x);
	result.w.x = (m2.w.x * m1.x.x) + (m2.w.y * m1.y.x) + (m2.w.z * m1.z.x) + (m2.w.w * m1.w.x);
	
	//row 2
	result.x.y = (m2.x.x * m1.x.y) + (m2.x.y * m1.y.y) + (m2.x.z * m1.z.y) + (m2.x.w * m1.w.y);
	result.y.y = (m2.y.x * m1.x.y) + (m2.y.y * m1.y.y) + (m2.y.z * m1.z.y) + (m2.y.w * m1.w.y);
	result.z.y = (m2.z.x * m1.x.y) + (m2.z.y * m1.y.y) + (m2.z.z * m1.z.y) + (m2.z.w * m1.w.y);
	result.w.y = (m2.w.x * m1.x.y) + (m2.w.y * m1.y.y) + (m2.w.z * m1.z.y) + (m2.w.w * m1.w.y);

	//row 3
	result.x.z = (m2.x.x * m1.x.z) + (m2.x.y * m1.y.z) + (m2.x.z * m1.z.z) + (m2.x.w * m1.w.z);
	result.y.z = (m2.y.x * m1.x.z) + (m2.y.y * m1.y.z) + (m2.y.z * m1.z.z) + (m2.y.w * m1.w.z);
	result.z.z = (m2.z.x * m1.x.z) + (m2.z.y * m1.y.z) + (m2.z.z * m1.z.z) + (m2.z.w * m1.w.z);
	result.w.z = (m2.w.x * m1.x.z) + (m2.w.y * m1.y.z) + (m2.w.z * m1.z.z) + (m2.w.w * m1.w.z);

	//row 4
	result.x.w = (m2.x.x * m1.x.w) + (m2.x.y * m1.y.w) + (m2.x.z * m1.z.w) + (m2.x.w * m1.w.w);
	result.y.w = (m2.y.x * m1.x.w) + (m2.y.y * m1.y.w) + (m2.y.z * m1.z.w) + (m2.y.w * m1.w.w);
	result.z.w = (m2.z.x * m1.x.w) + (m2.z.y * m1.y.w) + (m2.z.z * m1.z.w) + (m2.z.w * m1.w.w);
	result.w.w = (m2.w.x * m1.x.w) + (m2.w.y * m1.y.w) + (m2.w.z * m1.z.w) + (m2.w.w * m1.w.w);
	
	return result;
}

/*
	finding the matrix of minors
*/
mat4 m_minor(mat4 m){
	
	mat4 result;
	
	//top column
	result.x.x = (m.y.y * m.z.z * m.w.w) + (m.y.z * m.z.w * m.w.y) + (m.y.w * m.z.y * m.w.z) - (m.y.w * m.z.z * m.w.y) - (m.y.z * m.z.y * m.w.w) - (m.y.y * m.z.w * m.w.z);
	result.x.y = (m.y.x * m.z.z * m.w.w) + (m.y.z * m.z.w * m.w.x) + (m.y.w * m.z.x * m.w.z) - (m.y.w * m.z.z * m.w.x) - (m.y.z * m.z.x * m.w.w) - (m.y.x * m.z.w * m.w.z);
	result.x.z = (m.y.x * m.z.y * m.w.w) + (m.y.y * m.z.w * m.w.x) + (m.y.w * m.z.x * m.w.y) - (m.y.w * m.z.y * m.w.x) - (m.y.y * m.z.x * m.w.w) - (m.y.x * m.z.w * m.w.y);
	result.x.w = (m.y.x * m.z.y * m.w.z) + (m.y.y * m.z.z * m.w.x) + (m.y.z * m.z.x * m.w.y) - (m.y.z * m.z.y * m.w.x) - (m.y.y * m.z.x * m.w.z) - (m.y.x * m.z.z * m.w.y);

	//second column
	result.y.x = (m.x.y * m.z.z * m.w.w) + (m.x.z * m.z.w * m.w.y) + (m.x.w * m.z.y * m.w.z) - (m.x.w * m.z.z * m.w.y) - (m.x.z * m.z.y * m.w.w) - (m.x.y * m.z.w * m.w.z);
	result.y.y = (m.x.x * m.z.z * m.w.w) + (m.x.z * m.z.w * m.w.x) + (m.x.w * m.z.x * m.w.z) - (m.x.w * m.z.z * m.w.x) - (m.x.z * m.z.x * m.w.w) - (m.x.x * m.z.w * m.w.z);
	result.y.z = (m.x.x * m.z.y * m.w.w) + (m.x.y * m.z.w * m.w.x) + (m.x.w * m.z.x * m.w.y) - (m.x.w * m.z.y * m.w.x) - (m.x.y * m.z.x * m.w.w) - (m.x.x * m.z.w * m.w.y);
	result.y.w = (m.x.x * m.z.y * m.w.z) + (m.x.y * m.z.z * m.w.x) + (m.x.z * m.z.x * m.w.y) - (m.x.z * m.z.y * m.w.x) - (m.x.y * m.z.x * m.w.z) - (m.x.x * m.z.z * m.w.y);
	
	//third column
	result.z.x = (m.x.y * m.y.z * m.w.w) + (m.x.z * m.y.w * m.w.y) + (m.x.w * m.y.y * m.w.z) - (m.x.w * m.y.z * m.w.y) - (m.x.z * m.y.y * m.w.w) - (m.x.y * m.y.w * m.w.z);
	result.z.y = (m.x.x * m.y.z * m.w.w) + (m.x.z * m.y.w * m.w.x) + (m.x.w * m.y.x * m.w.z) - (m.x.w * m.y.z * m.w.x) - (m.x.z * m.y.x * m.w.w) - (m.x.x * m.y.w * m.w.z);
	result.z.z = (m.x.x * m.y.y * m.w.w) + (m.x.y * m.y.w * m.w.x) + (m.x.w * m.y.x * m.w.y) - (m.x.w * m.y.y * m.w.x) - (m.x.y * m.y.x * m.w.w) - (m.x.x * m.y.w * m.w.y);
	result.z.w = (m.x.x * m.y.y * m.w.z) + (m.x.y * m.y.z * m.w.x) + (m.x.z * m.y.x * m.w.y) - (m.x.z * m.y.y * m.w.x) - (m.x.y * m.y.x * m.w.z) - (m.x.x * m.y.z * m.w.y);
	
	//fourth column
	result.w.x = (m.x.y * m.y.z * m.z.w) + (m.x.z * m.y.w * m.z.y) + (m.x.w * m.y.y * m.z.z) - (m.x.w * m.y.z * m.z.y) - (m.x.z * m.y.y * m.z.w) - (m.x.y * m.y.w * m.z.z);
	result.w.y = (m.x.x * m.y.z * m.z.w) + (m.x.z * m.y.w * m.z.x) + (m.x.w * m.y.x * m.z.z) - (m.x.w * m.y.z * m.z.x) - (m.x.z * m.y.x * m.z.w) - (m.x.x * m.y.w * m.z.z);
	result.w.z = (m.x.x * m.y.y * m.z.w) + (m.x.y * m.y.w * m.z.x) + (m.x.w * m.y.x * m.z.y) - (m.x.w * m.y.y * m.z.x) - (m.x.y * m.y.x * m.z.w) - (m.x.x * m.y.w * m.z.y);
	result.w.w = (m.x.x * m.y.y * m.z.z) + (m.x.y * m.y.z * m.z.x) + (m.x.z * m.y.x * m.z.y) - (m.x.z * m.y.y * m.z.x) - (m.x.y * m.y.x * m.z.z) - (m.x.x * m.y.z * m.z.y);

	return result;
}

/* 
	turning matrix of minor into matrix of cofactor
*/
mat4 cofactor(mat4 m){
	
	mat4 result;
	
	result.x.x = m.x.x;
	result.x.y = (-1) * m.x.y;
	result.x.z = m.x.z;
	result.x.w = (-1) * m.x.w;
	
	result.y.x = (-1) * m.y.x;
	result.y.y = m.y.y;
	result.y.z = (-1) * m.y.z;
	result.y.w = m.y.w;
	
	result.z.x = m.z.x;
	result.z.y = (-1) * m.z.y;
	result.z.z = m.z.z;
	result.z.w = (-1) * m.z.w;
	
	result.w.x = (-1) * m.w.x;
	result.w.y = m.w.y;
	result.w.z = (-1) * m.w.z;
	result.w.w = m.w.w;
	
	return result;
}

/*
	determinant of a matrix
*/
float determinant(mat4 m, mat4 cofactor){
	
	float result, a, b, c, d;
	
	a = m.x.x * cofactor.x.x;
	b = m.x.y * cofactor.x.y;
	c = m.x.z * cofactor.x.z;
	d = m.x.w * cofactor.x.w;
	
	result = a + b + c + d;
	return result;
}

/*
	produce the transpose of the given matrix
*/
mat4 transpose(mat4 m){
	
	mat4 result;
	
	result.x.x = m.x.x;
	result.x.y = m.y.x;
	result.x.z = m.z.x;
	result.x.w = m.w.x;
	
	result.y.x = m.x.y;
	result.y.y = m.y.y;
	result.y.z = m.z.y;
	result.y.w = m.w.y;
	
	result.z.x = m.x.z;
	result.z.y = m.y.z;
	result.z.z = m.z.z;
	result.z.w = m.w.z;
	
	result.w.x = m.x.w;
	result.w.y = m.y.w;
	result.w.z = m.z.w;
	result.w.w = m.w.w;
	
	return result;
}

/*
	find the inverse of the given matrix 
*/
mat4 inverse(mat4 m){
	mat4 minor = m_minor(m);
	mat4 cof = cofactor(minor);
	mat4 trans = transpose(cof);
	float det = determinant(m, cof);
	mat4 result = scalar_x_matrix(trans, (1/det));
	return result;
}

/*
	given a matrix m and a vector v multiply the two together
	and return the resulting vector
*/
vec4 matrix_x_vector(mat4 m, vec4 v){
	
	vec4 result;
	
	result.x = ((m.x.x * v.x) + (m.y.x * v.y) + (m.z.x * v.z) + (m.w.x * v.w));
	result.y = ((m.x.y * v.x) + (m.y.y * v.y) + (m.z.y * v.z) + (m.w.y * v.w));
	result.z = ((m.x.z * v.x) + (m.y.z * v.y) + (m.z.z * v.z) + (m.w.z * v.w));
	result.w = ((m.x.w * v.x) + (m.y.w * v.y) + (m.z.w * v.z) + (m.w.w * v.w));
	
	return result;
}

/******************* GEOMETRIC OBJECTS AND TRANSFORMATIONS ********************/

/*
	translation: given the vec4 dd (distance&direction), create a translation matrix
	given the formula from the slides and multiply all of the vectors in the matrix object you want to translate by that. return the resulting matrix
*/

mat4 identity(){
	mat4 result = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	return result;
}
mat4 translate(float dx, float dy, float dz){
	
	mat4 translation = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {dx,dy,dz,1}};
	return translation;
}

//rotating about the x, y or z axis
mat4 rotatez(float angle){
	
	float rad = (M_PI / 180)*angle;
	mat4 result;
	
	result.x.x = cos(rad);	result.y.x = (-1)*sin(rad);	result.z.x = 0;	result.w.x = 0;
	result.x.y = sin(rad);	result.y.y = cos(rad);		result.z.y = 0;	result.w.x = 0;
	result.x.z = 0;			result.y.z = 0;				result.z.z = 1;	result.w.z = 0;
	result.x.w = 0;			result.y.w = 0;				result.z.w = 0;	result.w.w = 1;
	
	return result;
}
mat4 rotatex(float angle){
	
	float rad = (M_PI / 180)*angle;
	mat4 result;
	
	result.x.x = 1;			result.y.x = 0;				result.z.x = 0;			result.w.x = 0;
	result.x.y = 0;			result.y.y = cos(rad);		result.z.y = (-1)*sin(rad);	result.w.x = 0;
	result.x.z = 0;			result.y.z = sin(rad);		result.z.z = cos(rad);	result.w.z = 0;
	result.x.w = 0;			result.y.w = 0;				result.z.w = 0;			result.w.w = 1;
	
	return result;
}
mat4 rotatey(float angle){
	
	float rad = (M_PI / 180)*angle;
	mat4 result;
	
	result.x.x = cos(rad);	result.y.x = 0;			result.z.x = sin(rad);		result.w.x = 0;
	result.x.y = 0;			result.y.y = 1;			result.z.y = 0;				result.w.x = 0;
	result.x.z = (-1)*sin(rad);	result.y.z = 0;		result.z.z = cos(rad);		result.w.z = 0;
	result.x.w = 0;			result.y.w = 0;			result.z.w = 0;				result.w.w = 1;
	
	return result;
}

//scaling the matrix
mat4 scale(float x, float y, float z){
	
	mat4 scale = {{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}};
	return scale;
	
}

//look_at function for viewing
mat4 look_at(float eyex, float eyey, float eyez, 
	float atx, float aty, float atz,
	float upx, float upy, float upz);

