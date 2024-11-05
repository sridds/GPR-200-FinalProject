#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 WorldNormal;
out vec2 TexCoord;
out vec3 WorldPos;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
    vec4 worldPos = _Model * vec4(aPos,1.0);
	WorldPos = worldPos.xyz;
	WorldNormal = mat3(transpose(inverse(_Model))) * aNormal;

	TexCoord = aUV;
	gl_Position =_ViewProjection * worldPos;
}