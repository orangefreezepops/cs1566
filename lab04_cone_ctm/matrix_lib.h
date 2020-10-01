/*
author: Keegan Fouse
Matrix Library header file for cs1566
Intro to Computer Graphics
*/


/**************************** VECTOR & MATRIX STRUCTURES *************************/

/* 4x1 vector structure using floating point numbers as it's elements*/
typedef struct
{
  float x;
  float y;
  float z;
  float w;
} vec4;

/* 4x4 matrix structure using vctors as it's elements */
typedef struct 
{
	vec4 x;
	vec4 y;
	vec4 z;
	vec4 w;
} mat4;

/* (x, y) coordinate for rotations */
typedef struct
{
	float x;
	float y;
	float z;
} point;

/****************************** VECTOR OPERATIONS *********************************/

/*
	prints the 4x1 column vector to the consol
	to simplify print the vector like a row and 
	limit the decimal places in the float
*/
void vec_print(vec4 v);

/*
	return a vec4 object that is the result of a scalar (s)
	and the vec4 object (v)
*/
vec4 scalar_vec_mult(vec4 v, float s);

/*
	add two vectors together and return the result vector
*/
vec4 add_vec(vec4 v1, vec4 v2);

/*
	given two vectors v1, v2, subtract vector 2 from vector 1
	return the result vector
*/
vec4 sub_vec(vec4 v1, vec4 v2);

/*
	do the dot product operation of two vectors and return the
	resulting scalar 
*/
float dot_product(vec4 v1, vec4 v2);

/*
	given two vectors v1, v2 calculate the cross product for the first 3
	elements in the vector (x, y, z) and ignore the last (w)
	return the resulting vector with the 4th element equal to 0
*/
vec4 cross_product(vec4 v1, vec4 v2);

/*
	magnitude of a vector
*/
float magnitude(vec4 v);

/*
	normalize of a vector
*/
vec4 normalize(vec4 v);

/************************** MATRIX OPERATIONS ********************************/

/*
	print the matrix to the consol for debugging purposes
*/
void matrix_print(mat4 m);

/*
	scalar matrix multiplication, result should be a a matrix 
*/
mat4 scalar_x_matrix(mat4 m, float s);

/*
	given two matrices add them together and return the result matrix
*/
mat4 add_matrix(mat4 m1, mat4 m2);

/*
	given two matrices, subtract matrix 2 from matrix 1, return the 
	result
*/
mat4 sub_matrix(mat4 m1, mat4 m2);

/*
	given two matrices multiply them
*/
mat4 mult_matrix(mat4 m1, mat4 m2);

/*
	find the inverse of the given matrix 
*/
mat4 inverse(mat4 m);

/*
	helper functions for the inverse
*/
mat4 m_minor(mat4 m);
mat4 cofactor(mat4 m);
float determinant(mat4 m1, mat4 m2);

/*
	produce the transpose of the given matrix
*/
mat4 transpose(mat4 m);

/*
	given a matrix m and a vector v multiply the two together
	and return the resulting vector
*/
vec4 matrix_x_vector(mat4 m, vec4 v);

/******************* GEOMETRIC OBJECTS AND TRANSFORMATIONS ********************/

//scaling, translation, rotat x y and z
mat4 m4_identity();

mat4 translate(float x, float y, float z);

mat4 rotatex(float angle);
mat4 rotatey(float angle);
mat4 rotateZ(float angle);

mat4 scale(vec4 factor, mat4 m);

//look_at function

mat4 look_at(float eyex, float eyey, float eyez, 
	float atx, float aty, float atz,
	float upx, float upy, float upz);



















	