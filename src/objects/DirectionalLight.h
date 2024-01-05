#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include <glm/glm.hpp>

using namespace glm;

class DirectionalLight {
private:
    vec3 direction = vec3(0.f);

    vec3 ambient = vec3(0.f);
    vec3 diffuse = vec3(0.f);
    vec3 specular = vec3(0.f);
public:
    DirectionalLight(vec3, vec3, vec3, vec3);
    vec3 getDirection() { return direction; };
    vec3 geAmbient() { return ambient; };
    vec3 getDiffuse() { return diffuse; };
    vec3 getSpecular() { return specular; };
};

#endif // DIR_LIGHT_H