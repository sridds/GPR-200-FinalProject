#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	gl_Position =_ViewProjection * _Model * vec4(aPos,1.0);
}