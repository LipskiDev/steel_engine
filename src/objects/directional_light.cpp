#include "directional_light.h"

DirectionalLight::DirectionalLight(vec3 light_direction, vec3 light_ambient, vec3 light_diffuse, vec3 light_specular)
{
    direction_ = light_direction;
    ambient_ = light_ambient;
    diffuse_ = light_diffuse;
    specular_ = light_specular;
}
DirectionalLight::DirectionalLight()
{
    
}
