#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "scenes/TerrainGenerationScene.cpp"
#include "ui/Display.h"
#include "utils/SceneManager.h"



int main() {    

    SceneManager *sceneManager = SceneManager::instance();
    Display *display = new Display(1920, 1080, "Steel Engine");

    std::shared_ptr<TerrainGenerationScene> terrainGenerationScene = std::make_shared<TerrainGenerationScene>(display);

    sceneManager->add("Terrain Generation", terrainGenerationScene);
    sceneManager->switchTo("Terrain Generation");

    while(!display->shouldClose()) {
        glfwPollEvents();

        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        display->updateDeltaTime();
        float deltaTime = display->getDeltaTime();

        sceneManager->processInput();
        sceneManager->update(deltaTime);
        sceneManager->lateUpdate(deltaTime);
        sceneManager->draw(display);

        display->drawImGuis();

        glfwSwapBuffers(display->getWindow());
    }

    return 0;
}