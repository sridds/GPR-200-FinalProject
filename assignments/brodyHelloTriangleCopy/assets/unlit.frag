#version 330 core
out vec4 FragColor;

in vec4 Color;
uniform float _Time;

void main(){
	FragColor = vec4(Color.rgb * (sin(_Time)*0.5+0.5),1.0);
}