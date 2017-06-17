#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
  
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

out vec4 vFragColor;

in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // ambient
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 lightDirection = normalize(lightPosition);
    vec3 diffuse = light.diffuse * (max(dot(Normal, lightDirection), 0.0) * material.diffuse);
    
    // specular
    vec3 viewDirection = normalize(cameraPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, Normal);  
    vec3 specular = light.specular * (pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * material.specular);  
        
    vFragColor = vec4(ambient + diffuse + specular, 1.0);
}