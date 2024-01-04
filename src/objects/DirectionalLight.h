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
};