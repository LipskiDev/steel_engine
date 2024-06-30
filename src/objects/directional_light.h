#ifndef DIR_LIGHT_H
#define DIR_LIGHT_H

#include <glm/glm.hpp>

using namespace glm;

class DirectionalLight {
private:
    vec3 direction_ = vec3(0.f);

    vec3 ambient_ = vec3(0.f);
    vec3 diffuse_ = vec3(0.f);
    vec3 specular_ = vec3(0.f);
public:
    DirectionalLight();
    DirectionalLight(vec3, vec3, vec3, vec3);
    vec3 GetDirection() { return direction_; };
    vec3 GeAmbient() { return ambient_; };
    vec3 GetDiffuse() { return diffuse_; };
    vec3 GetSpecular() { return specular_; };
};

#endif // DIR_LIGHT_H