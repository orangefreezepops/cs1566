#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;
varying vec4 color;

uniform mat4 ctm;
uniform vec4 light_position;
uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;
uniform float shininess;
uniform float attenuation_constant;
uniform float attenuation_linear;
uniform float attenuation_quadratic;
vec4 ambient, diffuse, specular;

void main()
{
	ambient = ambient_product;
	
		vec4 N = normalize(ctm * vNormal);
	
		vec4 L_temp = (light_position - ctm * vPosition);
		vec4 L = normalize(L_temp);
		diffuse = max(dot(L,N), 0.0) * diffuse_product;
	
		vec4 eye_point = vec4(0.0, 0.0, 0.0, 1.0);
		vec4 V = normalize(eye_point - (vPosition));
		vec4 H = normalize(L + V);
		specular = pow(max(dot(N, H), 0.0), shininess) * specular_product;
		
		float distance = length(L_temp);
		float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) +
		(attenuation_quadratic * distance * distance));
		gl_Position = ctm * vPosition;
		//color = ambient + (diffuse + specular);
		color = vColor;
}

