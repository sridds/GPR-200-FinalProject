#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
uniform float _Time;

void main(){
    vec3 pos = aPos * 0.5;
	pos.y-=0.5;
	pos.y+=abs(sin(_Time*2.0))*0.5;
	gl_Position = vec4(pos,1.0);
	TexCoord = aTexCoord;
}