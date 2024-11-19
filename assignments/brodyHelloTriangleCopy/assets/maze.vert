#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 Color;
uniform float _Time;
uniform float posX;
uniform float posY;

void main(){
    vec3 pos = aPos;
	gl_Position = vec4((pos.x + posX), (pos.y + posY), pos.z,1.0);
	Color = aColor;
}