/*
	author: Keegan Fouse
	Matrix Library for CS1566
	Intro to Computer Graphics
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix_lib.h"

int main(void){
	
	vec4 v1 = {1, 2, 3, 4};
	vec4 v2 = {5, 6, 7, 8};
	vec4 v3 = {1,2,-3, 1};
	mat4 m1 = {{1, -5, 9, 13}, {2, 6, -10, 14}, {3, 7, 11, 15}, {4, 8, 12, -16}};
	mat4 m2 = {{4, 8, 12, 16}, {3, 7, 11, 15}, {2, 6, 10, 14}, {1, 5, 9, 13}};
	mat4 m3 = {{-1, 2, 1, 1}, {1, 5, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}};
	mat4 i = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
	float scalar = 3.0;
	/*
	mat4 inv2 = inverse(m2);
	printf("inverse of m2: \n");
	matrix_print(inv2);
	printf("\n");
	
	//scalar vector multiplication
	vec4 scal_vec = scalar_vec_mult(v1, scalar);
	printf("1.) Scalar-Vector multiplication\n");
	vec_print(scal_vec);
	printf("\n");
	
	//vector addition
	vec4 add = add_vec(v1, v2);
	printf("2.) Vector addition\n");
	vec_print(add);
	printf("\n");
	printf("DOT PRODUCT OF V1 (V1 + V2):\n");
	
	printf("%.2f\n", dot_product(v1, add));
	printf("\n");
	printf("%.2f\n", (dot_product(v1, v1) + dot_product(v1, v2)));
	
	//vector subtraction
	vec4 sub = sub_vec(v1, v2);
	printf("3.) Vector subtraction\n");
	vec_print(sub);
	printf("\n");
	
	printf("V1 - (V1 + V2) \n");
	vec_print(sub_vec(v1, add));
	
	//dot product
	printf("4.) Dot Product\n");
	printf("%.2f\n", dot_product(v1, v2));
	printf("\n");
	
	//cross product
	vec4 cross = cross_product(v1, v2);
	printf("5.) Cross Product\n");
	vec_print(cross);
	printf("DOT PRODUCT OF CROSS PRODUCT AND V1: \n");
	printf("%.2f\n", dot_product(v1, cross));
	printf("\n");
	
	//scalar matrix multiplication
	mat4 smat = scalar_x_matrix(m1, scalar);
	printf("6.) Scalar-Matrix multiplication\n");
	matrix_print(smat);
	printf("\n");
	
	//matrix addition
	mat4 madd = add_matrix(m1, m2);
	printf("7.) Matrix addition\n");
	matrix_print(madd);
	printf("\n");
	
	//matrix subtraction
	mat4 smub = sub_matrix(m1, m2);
	printf("8.) Matrix subtraction\n");
	matrix_print(smub);
	printf("\n");
	
	//matrix multiplication
	mat4 mult = mult_matrix(m1, m2);
	printf("9.) Matrix multiplication\n");
	matrix_print(mult);
	printf("\n");
	
	//inverse
	mat4 inv = inverse(m1);
	printf("10.) Inverse\n");
	matrix_print(inv);
	printf("\n");
	
	//transpose
	mat4 trans = transpose(m1);
	printf("11.) Transpose\n");
	matrix_print(trans);
	printf("\n");
	
	//matrix vector multiplication
	vec4 mxv = matrix_x_vector(m1, v1);
	printf("12.) Matrix-Vector multiplication\n");
	vec_print(mxv);
	printf("\n");
	
	mat4 m3 = scalar_x_matrix(m1, 2);
	printf("Matrix 3: \n");
	matrix_print(m3);
	printf("is 2 times m1: \n");
	matrix_print(m1);
	printf("\n");
	
	//vector magnitude
	printf("magnitude of v1: %.2f\n", magnitude(v1));
	
	//vector normalize
	printf("\nnormalize of vector v1: \n");
	vec_print(normalize(v1));
	printf("\n");
	
	//inverse of i
	printf("i\n");
	matrix_print(i);
	printf("\n");
	printf("inverse of i: \n");
	matrix_print(inverse(i));
	printf("\n");
	*/
	printf("Testing translation function: \n");
	matrix_print(m3);
	printf("\n");
	matrix_print(translate(m3, v3));
	printf("\n");
	
	printf("testing scaling function\n");
	matrix_print(scale(v3, m1));
	
}