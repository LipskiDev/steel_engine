// Fragment Shader
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

/* Directional Light
struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
*/

// Point Light
struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
  
uniform Material material;
uniform Light light;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;



void main()
{


    // Ambient light
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // Diffuse Light
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
            
    // Specular Light
    vec3 viewDir =  normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
} 