#version 330 core
out vec4 FragColor;

in vec3 WorldNormal;
in vec3 WorldPos;

in vec2 TexCoord;
uniform sampler2D _MainTex;

uniform vec3 _LightColor = vec3(1);
uniform vec3 _LightPos = vec3(0);
uniform vec3 _ViewPos;
struct Material{
    float ambientK;
    float diffuseK;
    float specularK;
    float shininess;
    int blinnPhong;
};
uniform Material _Material;

//Toon Shading
uniform bool _ToonShadingEnabled;
uniform bool _RimLightingEnabled;

uniform int _ToonLevels = 4;
float _ToonScale = 1.0 / _ToonLevels;

void main(){
    // ambient
    float ambientStrength =_Material.ambientK;
    vec3 ambient = ambientStrength * _LightColor;
  	
    // diffuse 
    vec3 norm = normalize(WorldNormal);
    vec3 lightDir = normalize(_LightPos - WorldPos);
    float diffIntensity = max(dot(norm, lightDir), 0.0);

    if (_ToonShadingEnabled)
    {
        diffIntensity = floor(diffIntensity * _ToonLevels) * _ToonScale;
    }
   
    vec3 diffuseColor = _LightColor * _Material.diffuseK * diffIntensity;

    //Take dot product of two vectors and put into cosine
    float shadingIntensity = cos(dot(norm, lightDir));

    // specular
    float specularStrength = _Material.specularK;

    vec3 viewDir = normalize(_ViewPos - WorldPos);
    //vec3 halfwayDir = normalize(lightDir + viewDir);  
    //specularStrength = clamp(dot(norm, halfwayDir), 0.0, 1.0);
    //specularStrength = step(0.98, specularStrength);
    
    //vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = 0;
    if(_Material.blinnPhong == 1)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0),  _Material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0),  _Material.shininess);
    }
    vec3 specular = specularStrength * spec * _LightColor;  
    vec3 result = (ambient + diffuseColor + specular) * texture(_MainTex,TexCoord).rgb;
	FragColor = vec4(result,1.0);
}