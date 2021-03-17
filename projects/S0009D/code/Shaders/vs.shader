#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 view;
uniform mat4 position;
uniform mat4 rotation;
uniform mat4 scale;

out vec2 TexCoord;

void main()
{
	mat4 model = position * rotation * scale;
	gl_Position = view * model * vec4(pos, 1.0f);
	TexCoord = aTexCoord;
}