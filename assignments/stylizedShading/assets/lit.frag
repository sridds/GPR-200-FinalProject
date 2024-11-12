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

void main(){
    // ambient
    float ambientStrength =_Material.ambientK;
    vec3 ambient = ambientStrength * _LightColor;
  	
    // diffuse 
    vec3 norm = normalize(WorldNormal);
    vec3 lightDir = normalize(_LightPos - WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * _LightColor * _Material.diffuseK;

    //Take dot product of two vectors and put into cosine
    float shadingIntensity = cos(dot(norm, lightDir));
    

    // specular
    float specularStrength = _Material.specularK;
    vec3 viewDir = normalize(_ViewPos - WorldPos);
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
    vec3 result = (ambient + diffuse + specular) * texture(_MainTex,TexCoord).rgb;

	//FragColor =;
	FragColor = vec4(result,1.0);
}