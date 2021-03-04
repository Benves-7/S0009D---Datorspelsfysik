#version 430
layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;
layout(location=2) in vec2 texCords;
layout(location=3) in vec3 normal;

out vec3 position;
out vec2 texCords2;
out vec3 uNormal;
out vec4 Color;
out vec4 LightPos;
out vec4 LightColor;
out vec4 CameraPosition;

uniform mat4 transform;
uniform mat4 objPosition;
uniform vec4 lightPos;
uniform vec4 lightColor;
uniform vec4 cameraPosition;
void main()
{
	gl_Position = vec4(pos, 1.0f);
	texCords2 = texCords;	
	Color = color;
	uNormal = mat3(transpose(inverse(objPosition))) * normal;
	LightPos = lightPos;
	LightColor = lightColor;
	position = (objPosition * vec4(pos, 1.0f)).xyz;
	
	CameraPosition = cameraPosition;
}