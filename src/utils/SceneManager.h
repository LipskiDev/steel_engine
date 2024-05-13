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

    uint64_t add(std::shared_ptr<Scene> scene);
    void switchTo(uint64_t sceneId);
    void remove(uint64_t sceneId);

private:
    std::unordered_map<uint64_t, std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> curScene;
    uint64_t insertedSceneId = 0;
    
};

#endif // SCENE_MANAGER_H