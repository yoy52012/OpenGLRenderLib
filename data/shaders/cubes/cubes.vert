#version 450

in vec3 a_position;
in vec4 a_color;

out vec4 v_color;

uniform mat4 u_modelViewProjectionMatrix;

void main()
{
    gl_Position = u_modelViewProjectionMatrix * vec4(a_position, 1.0f);
    v_color = a_color;
}