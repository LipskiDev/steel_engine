// Fragment Shader
#version 410 core

layout (location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform float maxHeight;
uniform float minHeight;

// Perlin Noise
uniform float meshHeight;
uniform float waterHeight;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 palette(float t);
float mapHeight(float x);
vec3 getColor(float height);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    FragColor = vec4(getColor(FragPos.y), 1.0);
}

vec3 getColor(float height) {
    if(height <= waterHeight * 0.5 * meshHeight) { 
        return vec3(0.23, 0.37, 0.75);                  // Deep Water
    } else if(height <= waterHeight * meshHeight) {
        return vec3(0.23, 0.4, 0.75);                   // Shallow Water
    } else if(height <= 0.15 * meshHeight) {
        return vec3(0.82, 0.84, 0.51);                  // Sand
    } else if(height <= 0.3 * meshHeight) {
        return vec3(0.37, 0.65, 0.12);                  // Grass 1
    } else if(height <= 0.4 * meshHeight) {
        return vec3(0.25, 0.45, 0.08);                  // Grass 2
    } else if(height <= 0.5 * meshHeight) {
        return vec3(0.35, 0.25, 0.23);                  // Rock 1
    } else if(height <= 0.8 * meshHeight) {
        return vec3(0.29, 0.23, 0.21);                  // Rock 2
    } else {
        return vec3(1.0, 1.0, 1.0);                     // Snow
    }
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(0.8, 0.8, 0.5);
    vec3 d = vec3(0.0, 0.2, 0.5);
    return a + b*cos(6.28318*(c*t+d));
}

float mapHeight(float x) {
    return (x - minHeight) / (maxHeight - minHeight);
}
