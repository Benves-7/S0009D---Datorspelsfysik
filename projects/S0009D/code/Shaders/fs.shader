#version 430
out vec4 fragColor;
in vec2 texCords2;
in vec3 position;
in vec4 Color;
in vec3 uNormal;
in vec4 LightPos;
in vec4 LightColor;
in vec4 CameraPosition;
uniform sampler2D diffuser;
uniform float intensity;
void main()
{
	//Ambient light
	vec3 ambientLight = vec3(0.01f,0.01f,0.01f);
	
	//Diffuse light
	vec3 posToLightDirVec = normalize(vec3(LightPos) - position);
	vec3 diffuseColor = vec3(LightColor);
	
	float diffuse = clamp(dot(posToLightDirVec, uNormal),0,1);
	vec3 finalDiffuse = diffuseColor * diffuse * intensity;
	
	//Specular light
	vec3 lightToPosVec = normalize(vec3(LightPos) - position);
	vec3 reflectDirection = normalize(reflect(lightToPosVec, normalize(uNormal)));
	vec3 posToViewVec = normalize(position - vec3(CameraPosition));
	vec3 specFinal;
	if(dot(uNormal, lightToPosVec) < 0.0){
		specFinal = vec3(0,0,0);
	}else{
		float specularConst = pow(max(dot(posToViewVec, reflectDirection), 0), 30);
		specFinal = vec3(LightColor) * specularConst;
	}

	//Calculate final color per pixel
	fragColor = texture(diffuser, texCords2);
}