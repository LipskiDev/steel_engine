#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "scenes/terrain_generation_scene.cpp"
#include "scenes/fat_orc_scene.cpp"
#include "ui/display.h"
#include "utils/scene_manager.h"



int main() {    

    SceneManager *scene_manager = SceneManager::Instance();
    Display *display = new Display(1920, 1080, "Steel Engine");

    std::shared_ptr<TerrainGenerationScene> terrain_generation_scene = std::make_shared<TerrainGenerationScene>(display);

    scene_manager->Add("Terrain Generation Scene", terrain_generation_scene);
    scene_manager->SwitchTo("Terrain Generation Scene");

    while(!display->ShouldClose()) {
        glfwPollEvents();

        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        display->UpdateDeltaTime();
        float delta_time = display->GetDeltaTime();

        scene_manager->ProcessInput();
        scene_manager->Update(delta_time);
        scene_manager->LateUpdate(delta_time);
        scene_manager->Draw(display);

        display->DrawImGuis();

        glfwSwapBuffers(display->GetWindow());
    }

    return 0;
}