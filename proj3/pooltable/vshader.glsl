#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 ctm;
uniform vec4 light_position;
uniform int is_shadow;

void main()
{
	if (is_shadow == 0){
		gl_Position = model_view_matrix * ctm * vPosition;
		color = vColor;
	} else {
		vec4 newPosition = ctm * vPosition;
		
		float x = light_position.x - (light_position.y * ((light_position.x - newPosition.x)/(light_position.y - newPosition.y)));
		
		float z = light_position.z - (light_position.y * ((light_position.z - newPosition.z)/(light_position.y - newPosition.y)));
		
		gl_Position = model_view_matrix * vec4(x, 0.01, z, 1);
		color = vec4(0, 0, 0, 1);
	}
}
