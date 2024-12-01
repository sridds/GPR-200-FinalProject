#version 330 core
out vec4 FragColor;

in vec3 WorldNormal;
in vec3 WorldPos;

in vec2 TexCoord;

// textures[0] is the wall and textures[1] is the floor
uniform sampler2D textures[2];
uniform int _ActiveTexture;

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
uniform float _RimLightIntensity = 0.3; 

vec3 CalculateLighting();
vec3 CalculateRimLighting(vec3 viewDir, vec3 norm);
float CalculateFogFactor();

//Fog 
uniform float _FogStart = 5.0;
uniform float _FogEnd = 100.0;
uniform vec3 _FogColor = vec3(1);
uniform float _FogExponential = 2.0;

void main() {
    // deciding which sampler2d to use

    vec3 result = CalculateLighting();

    if (_FogColor != vec3(0)){
        float fogFactor = CalculateFogFactor();
        result = mix(_FogColor, result, pow(fogFactor, _FogExponential));
    }

	FragColor = vec4(result, 1.0);
}

float CalculateFogFactor(){
    float cameraToPixel = length(WorldPos - _ViewPos);
    float fogRange = _FogEnd - _FogStart;
    float fogDistance = _FogEnd - cameraToPixel;
    float fogFactor = fogDistance / fogRange;
    return clamp(fogFactor, 0.0, 1.0);
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
        vec3 rimFactor = CalculateRimLighting(viewDir, norm);
        return (ambient + diffuseColor + specular + rimFactor) * texture(textures[_ActiveTexture],TexCoord).rgb;
    }
    
    return (ambient + diffuseColor + specular) * texture(textures[_ActiveTexture],TexCoord).rgb;
}

vec3 CalculateRimLighting(vec3 viewDir, vec3 norm)
{
    float rimFactor = dot(viewDir, norm);

    //Swap dot product so rimFactor is max when perpendicular
    rimFactor = 1.0 - rimFactor;
    rimFactor = max(0.0, rimFactor);
    rimFactor = pow(rimFactor, _RimLightFalloff);

    //Return the rimFactor multiplied by current light color and intensity
    return rimFactor * _RimLightIntensity * _LightColor;
}