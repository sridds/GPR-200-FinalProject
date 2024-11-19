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
    bool blinnPhong;
};
uniform Material _Material;

//Toon Shading
uniform bool _ToonShadingEnabled;
uniform bool _RimLightingEnabled = true;

uniform int _ToonLevels = 4;
float _ToonScale = 1.0 / _ToonLevels;

uniform float _RimLightFalloff = 4.0;

vec3 CalculateLighting();
float CalculateRimLighting(vec3 viewDir, vec3 norm);

void main(){
    
    vec3 result = CalculateLighting();

	FragColor = vec4(result,1.0);
}

vec3 CalculateLighting()
{
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
    
    //vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = 0;
    if(_Material.blinnPhong && !_ToonShadingEnabled)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0),  _Material.shininess);
    }
    else if(!_Material.blinnPhong && !_ToonShadingEnabled)
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0),  _Material.shininess);
    }
    else
    {
        //Toon shading specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        specularStrength = clamp(dot(norm, halfwayDir), 0.0, 1.0);
        specularStrength = step(0.98, specularStrength);
    }

    vec3 specular = specularStrength * spec * _LightColor;  

    if (_RimLightingEnabled)
    {
        float rimFactor = CalculateRimLighting(viewDir, norm);
        return (ambient + diffuseColor + specular + rimFactor) * texture(_MainTex,TexCoord).rgb;
    }
    
    return (ambient + diffuseColor + specular) * texture(_MainTex,TexCoord).rgb;
}

float CalculateRimLighting(vec3 viewDir, vec3 norm)
{
    float rimFactor = dot(viewDir, norm);

    //Swap dot product so rimFactor is max when perpendicular
    rimFactor = 1.0 - rimFactor;
    rimFactor = max(0.0, rimFactor);
    rimFactor = pow(rimFactor, _RimLightFalloff);
    return rimFactor;
}