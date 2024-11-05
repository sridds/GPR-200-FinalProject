#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float _Time;
uniform sampler2D _MainTex;

void main(){
//	FragColor = vec4(TexCoord,0.0,1.0);
	FragColor = texture(_MainTex,TexCoord);
}