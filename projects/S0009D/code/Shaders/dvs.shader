#version 430
layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;

uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

void main()
{
	gl_Position = view * model * vec4(pos, 1);
	Color = color;
}