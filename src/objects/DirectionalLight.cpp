#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(vec3 lightDirection, vec3 lightAmbient, vec3 lightDiffuse, vec3 lightSpecular)
{
    this->direction = lightDirection;
    this->ambient = lightAmbient;
    this->diffuse = lightDiffuse;
    this->specular = lightSpecular;
}