#ifndef SCENE_H
#define SCENE_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ui/display.h"

class Scene {
public:
    virtual void OnCreate() = 0;

    virtual void OnDestroy() = 0;

    virtual void OnActivate() = 0;

    virtual void OnDeactivate() = 0;

    virtual void ProcessInput() {};
    virtual void Update(float delta_time) {};
    virtual void LateUpdate(float delta_time) {};
    virtual void Draw(Display *window) {};
};

#endif // SCENE_H