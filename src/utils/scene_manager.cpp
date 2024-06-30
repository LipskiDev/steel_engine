#include "scene_manager.h"

SceneManager *SceneManager::scene_manager = nullptr;


SceneManager::SceneManager() : scenes_(0), curr_scene_(0) { 
    scenes_ = std::unordered_map<std::string, std::shared_ptr<Scene>>();
}

SceneManager* SceneManager::Instance()
{
    if(!scene_manager) scene_manager = new SceneManager();
    return scene_manager;
}

void SceneManager::ProcessInput()
{
    if(curr_scene_) {
        curr_scene_->ProcessInput();
    }
}

void SceneManager::Update(float delta_time)
{
    if(curr_scene_) {
        curr_scene_->Update(delta_time);
    }
}

void SceneManager::LateUpdate(float delta_time)
{
    if(curr_scene_) {
        curr_scene_->LateUpdate(delta_time);
    }
}

void SceneManager::Draw(Display *display)
{
    if(curr_scene_) {
        curr_scene_->Draw(display);
    }
}

void SceneManager::Add(std::string scene_name, std::shared_ptr<Scene> scene)
{
    auto inserted_scene = scenes_.insert(std::make_pair(scene_name, scene));
    inserted_scene.first->second->OnCreate();
}

void SceneManager::SwitchTo(std::string scene_name)
{
    auto scenes_it = scenes_.find(scene_name);
    if(scenes_it != scenes_.end()) {
        if(curr_scene_) {
            curr_scene_->OnDeactivate();
        }

        curr_scene_ = scenes_it->second;
        curr_scene_->OnActivate();
    }
}

void SceneManager::Remove(std::string scene_name)
{
    auto scenes_it = scenes_.find(scene_name);
    if(scenes_it != scenes_.end()) {
        if(curr_scene_ == scenes_it->second) {
            curr_scene_ = nullptr;
        } 
        scenes_it->second->OnDestroy();
        scenes_.erase(scenes_it);
    }
};
