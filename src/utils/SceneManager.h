#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Scene.h"

#include <unordered_map>


class SceneManager {
private:
    SceneManager();
    static SceneManager *sceneManager;
    

public:
    SceneManager(SceneManager &other) = delete;

    void operator=(const SceneManager &) = delete;

    static SceneManager *instance();



    void processInput();
    void update(float deltaTime);
    void lateUpdate(float deltaTime);
    void draw(Display *display);

    void add(std::string sceneName, std::shared_ptr<Scene> scene);
    void switchTo(std::string sceneName);
    void remove(std::string sceneName);

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> curScene;
    
};

#endif // SCENE_MANAGER_H