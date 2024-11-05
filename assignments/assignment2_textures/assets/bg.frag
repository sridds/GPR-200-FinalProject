#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float _Time;
uniform sampler2D _MainTex;
uniform sampler2D _DecalTex;

void main(){
    vec2 uv = TexCoord * 5.0;
    uv.y+=sin(uv.x*2.0)*0.5;
    uv.x+=_Time;
    vec4 bgColor = texture(_MainTex,uv);
    vec4 decalColor = texture(_DecalTex,uv);
    vec3 col = mix(bgColor.rgb,decalColor.rgb,decalColor.a * 0.5);
	FragColor = vec4(col,1.0);
}