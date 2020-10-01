kdf11@pitt.edu
Keegan Fouse
9/20/19

included: 
matrix_lib.c
matrix_lib.h
test.c

compile using:
gcc matrix_lib.c test.c -o test -lm

everything runs smoothly

These are the structures and methods used


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

void vec_print(vec4 v);
	pring the vector

vec4 scalar_vec_mult(vec4 v, float s);
	scalar vector multiplication
	
vec4 add_vec(vec4 v1, vec4 v2);
	add the two vectors

vec4 sub_vec(vec4 v1, vec4 v2);
	subtract the vectors

float dot_product(vec4 v1, vec4 v2);
	dot product of the two vectors
	
vec4 cross_product(vec4 v1, vec4 v2);
	cross product ofthe vector
	
float magnitude(vec4 v);
	find the magnitude of the vector

vec4 normalize(vec4 v);
	normalize the vector

void matrix_print(mat4 m);
	print the matrix
	
mat4 scalar_x_matrix(mat4 m, float s);
	matrix times a scalar
	
mat4 add_matrix(mat4 m1, mat4 m2);
	add the matrices given
	
mat4 sub_matrix(mat4 m1, mat4 m2);
	subtracting the matrixes given
	
mat4 mult_matrix(mat4 m1, mat4 m2);
	multiply two matrices given
	
mat4 inverse(mat4 m):
	inverses the matrix given
	
mat4 m_minor(mat4 m):
	produces matrix of minor
	
mat4 cofactor(mat4 m):
	produces matrix of cofactor
	
float determinant(mat4 m1, mat4 m2):
	produces the determinant of given matrix

mat4 transpose(mat4 m);
	produce the transpose of the given matrix
	
matrix_x_vector(mat4 m, vec4 v):

	given a matrix m and a vector v multiply the two together
	and return the resulting vector






















	