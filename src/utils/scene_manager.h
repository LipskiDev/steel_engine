#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "scene.h"

#include <unordered_map>


class SceneManager {
private:
    SceneManager();
    static SceneManager *scene_manager;
    

public:
    SceneManager(SceneManager &other) = delete;

    void operator=(const SceneManager &) = delete;

    static SceneManager *Instance();



    void ProcessInput();
    void Update(float delta_time);
    void LateUpdate(float delta_time);
    void Draw(Display *display);

    void Add(std::string scene_name, std::shared_ptr<Scene> scene);
    void SwitchTo(std::string scene_name);
    void Remove(std::string scene_name);

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;

    std::shared_ptr<Scene> curr_scene_;
    
};

#endif // SCENE_MANAGER_H