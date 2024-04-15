#include "SceneManager.h"

SceneManager *SceneManager::sceneManager = nullptr;


SceneManager::SceneManager() : scenes(0), curScene(0) { 
    scenes = std::unordered_map<uint64_t, std::shared_ptr<Scene>>();
}

SceneManager* SceneManager::getInstance()
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

uint64_t SceneManager::add(std::shared_ptr<Scene> scene)
{
    auto insertedScene = scenes.insert(std::make_pair(insertedSceneId, scene));
    ++insertedSceneId;
    insertedScene.first->second->onCreate();
    return insertedSceneId - 1;
    
}

void SceneManager::switchTo(uint64_t sceneId)
{
    auto scenesIt = scenes.find(sceneId);
    if(scenesIt != scenes.end()) {
        if(curScene) {
            curScene->onDeactivate();
        }

        curScene = scenesIt->second;
        curScene->onActivate();
    }
}

void SceneManager::remove(uint64_t sceneId)
{
    auto scenesIt = scenes.find(sceneId);
    if(scenesIt != scenes.end()) {
        if(curScene == scenesIt->second) {
            curScene = nullptr;
        } 
        scenesIt->second->onDestroy();
        scenes.erase(scenesIt);
    }
};
