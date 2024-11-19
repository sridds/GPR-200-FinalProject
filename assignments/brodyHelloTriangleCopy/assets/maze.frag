#version 330 core
out vec4 FragColor;

uniform float _Time;
uniform float colFloat;

void main()
{
	FragColor = vec4(colFloat, colFloat, colFloat, 1.0f);
}