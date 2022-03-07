#version 450

in vec3 vertex;
in vec3 color;

out vec3 outColor; 

void main(void)
{
	gl_Position = vec4(vertex, 1.0f);
	outColor = color;
}