#include "SceneManager.h"

SceneManager *SceneManager::sceneManager = nullptr;


SceneManager::SceneManager() : scenes(0), curScene(0) { 
    scenes = std::unordered_map<std::string, std::shared_ptr<Scene>>();
}

SceneManager* SceneManager::instance()
{
    if(!sceneManager) sceneManager = new SceneManager();
    return sceneManager;
}

void SceneManager::processInput()
{
    if(curScene) {
        curScene->processInput();
    }
}

void SceneManager::update(float deltaTime)
{
    if(curScene) {
        curScene->update(deltaTime);
    }
}

void SceneManager::lateUpdate(float deltaTime)
{
    if(curScene) {
        curScene->lateUpdate(deltaTime);
    }
}

void SceneManager::draw(Display *display)
{
    if(curScene) {
        curScene->draw(display);
    }
}

void SceneManager::add(std::string sceneName, std::shared_ptr<Scene> scene)
{
    auto insertedScene = scenes.insert(std::make_pair(sceneName, scene));
    insertedScene.first->second->onCreate(); 
}

void SceneManager::switchTo(std::string sceneName)
{
    auto scenesIt = scenes.find(sceneName);
    if(scenesIt != scenes.end()) {
        if(curScene) {
            curScene->onDeactivate();
        }

        curScene = scenesIt->second;
        curScene->onActivate();
    }
}

void SceneManager::remove(std::string sceneName)
{
    auto scenesIt = scenes.find(sceneName);
    if(scenesIt != scenes.end()) {
        if(curScene == scenesIt->second) {
            curScene = nullptr;
        } 
        scenesIt->second->onDestroy();
        scenes.erase(scenesIt);
    }
};
