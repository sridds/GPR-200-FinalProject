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
uniform bool _RimLightingEnabled;

uniform int _ToonLevels = 4;
float _ToonScale = 1.0 / _ToonLevels;

uniform float _RimLightFalloff = 4.0;
uniform float _RimLightIntensity = 0.3; 

vec3 CalculateLighting();
vec3 CalculateRimLighting(vec3 viewDir, vec3 norm);

//Pixelation
uniform bool _PixelationEnabled = true;
uniform float _WidthPixelation = 64;
uniform float _HeightPixelation = 64;
uniform float _ColorPrecision = 2;
vec3 color;

vec3 CalculatePixelation();

//Fog 
uniform bool _FogEnabled;
uniform float _FogStart = 5.0;
uniform float _FogEnd = 100.0;
uniform vec3 _FogColor = vec3(1);
uniform float _FogExponential = 2.0;

float CalculateFogFactor();

//Dithering
uniform int _DitherPattern;
uniform float _DitherThreshold = 0.5;
uniform float _DitherStrength = 1;
uniform float _DitherScale = 1;

mat4x4 ditherPaterns = mat4x4(0, 1, 0, 1,
                              1, 0, 1, 0,
                              0, 1, 0, 1,
                              1, 0, 1, 0);

float PixelBrightness(vec3 pixelColor);
vec4 GetTexelSize(float width, float height);
float GetDitherValue(vec2 uv, float brightness, mat4x4 pattern);

void main() {
    // deciding which sampler2d to use
    vec3 result = CalculateLighting();
    
    //Fog
    if (_FogEnabled){
        float fogFactor = CalculateFogFactor();
        result = mix(_FogColor, result, pow(fogFactor, _FogExponential));
    }

    //Pixelation
    if (_PixelationEnabled){
        result *= CalculatePixelation();
    }
    else{
        result = result * texture(textures[_ActiveTexture],TexCoord).rgb;
    }

	FragColor = vec4(result, 1.0);
}

//Returns the average color of a pixel
float PixelBrightness(vec3 pixelColor){
    return pixelColor.r + pixelColor.g + pixelColor.b / 3.0;
}

vec4 GetTexelSize(float width, float height){
    return vec4(1 / width, 1 / height, width, height);
}

float GetDitherValue(vec2 uv, float brightness, mat4x4 pattern){
    int x = int(mod(uv.x, 4.0));
    int y = int(mod(uv.y, 4.0));

    return brightness * _DitherThreshold < ditherPaterns[x][y] ? 0 : 1;
}

float CalculateDitherCoordinate(vec3 pixelColor){
    vec4 texelSize = GetTexelSize(1, 1);
    vec2 ditherCoordinate = texelSize.xy;
    ditherCoordinate /= _DitherScale;

    float brightness = PixelBrightness(pixelColor);
    float ditherPixel = GetDitherValue(TexCoord, brightness, ditherPaterns);
    //return pixelColor * ditherPixel;
    return 0;
}

//Returns a scaled texture UV and color value
vec3 CalculatePixelation(){
    vec2 uv = TexCoord;
    uv.x = floor(uv.x * _WidthPixelation) / _WidthPixelation;
    uv.y = floor(uv.y * _HeightPixelation) / _HeightPixelation;
    color = texture(textures[_ActiveTexture], uv).rgb;
    color = floor(color * _ColorPrecision) / _ColorPrecision;

    return color;
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
        return (ambient + diffuseColor + specular + rimFactor);
    }
    
    return (ambient + diffuseColor + specular);
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