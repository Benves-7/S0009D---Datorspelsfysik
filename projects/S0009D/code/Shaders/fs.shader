#version 430
in vec2 TexCoord;
in vec3 Normal;
in vec3 Pos;

in vec4 LightPosition;
in vec4 LightColor;
in vec4 CameraPosition;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}