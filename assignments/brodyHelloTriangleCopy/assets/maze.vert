#version 330 core
layout(location = 0) in vec3 aPos;

uniform float posX;
uniform float posY;
uniform float scaleX;

void main()
{
	vec3 scaledPos = vec3(aPos.x * scaleX, aPos.y, aPos.z);
    vec3 finalPos = scaledPos + vec3(posX, posY, 0.0);

    gl_Position = vec4(finalPos, 1.0);
}