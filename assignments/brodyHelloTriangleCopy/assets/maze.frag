#version 330 core
out vec4 FragColor;

uniform float colFloat;

in vec2 TexCoord;

void main()
{
    // color float from randomized color in main.cpp
    vec3 baseColor = vec3(colFloat, colFloat, colFloat);
    
    FragColor = vec4(baseColor, 1.0f);
}