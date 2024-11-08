#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 Color;
uniform float _Time;

void main(){
    vec3 pos = aPos;
	pos.y+=sin(pos.x+_Time)*0.2;
	gl_Position = vec4(pos,1.0);
	Color = aColor;
}