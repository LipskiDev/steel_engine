#ifndef SCENE_H
#define SCENE_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../ui/Display.h"

class Scene {
public:
    virtual void onCreate() = 0;

    virtual void onDestroy() = 0;

    virtual void onActivate() = 0;

    virtual void onDeactivate() = 0;

    virtual void processInput() {};
    virtual void update(float deltaTime) {};
    virtual void lateUpdate(float deltaTime) {};
    virtual void draw(Display *window) {};
};

#endif // SCENE_H