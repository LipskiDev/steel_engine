#ifndef TERRAIN_GENERATION_SCENE
#define TERRAIN_GENERATION_SCENE

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../utils/Scene.h"

#include "../math/Vector.h"

// GLM Imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../input/InputHandler.h"


#include "../terrain/PerlinNoiseChunkGenerator.h"
#include "../terrain/terrain.h"

#include "../utils/Shader.h"
#include "../ui/Display.h"
#include "../utils/Scene.h"
#include "../objects/Camera.h"

#include "../model/Model.h"

#include "../config.h"

#include <vector>

class TerrainGenerationScene : virtual public Scene {

private:
    std::unique_ptr<Camera> camera;
    std::unique_ptr<ShaderProgram> mainShader;
    std::unique_ptr<PerlinNoiseChunkGenerator> generator;

    float scale = 1.0f;

    glm::mat4 model = glm::mat4(1.0f);


public:
    Display *display;

    TerrainGenerationScene(Display *window) {
        display = window;
    }

    void onCreate() {
        mainShader = make_unique<ShaderProgram>(std::initializer_list<std::pair<std::string_view, Shader::Type>> {
            {ROOT_DIR"/assets/shaders/terrainVertex.glsl", Shader::Type::Vertex},
            {ROOT_DIR"/assets/shaders/terrainFragment.glsl", Shader::Type::Fragment}
        });
        mainShader->use();

        generator = make_unique<PerlinNoiseChunkGenerator>();

        camera = make_unique<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 90.f, 1920.f / 1080.f, 1.f, 1000.f);

        mainShader->set_float_uniform("waterHeight", generator->getWaterHeight());
        mainShader->set_float_uniform("meshHeight", generator->getMeshHeight());


        mainShader->set_float_uniform("scale", 1.0f);


    }

    void onDestroy() {

    }

    void onActivate() {
        InputHandler *inputHandler = InputHandler::instance();


        inputHandler->addKeyHoldCallback(GLFW_KEY_W, [this] () { moveCamera(Vector2D<int>{0, 1}); });
        inputHandler->addKeyHoldCallback(GLFW_KEY_A, [this] () { moveCamera(Vector2D<int>{-1, 0}); });
        inputHandler->addKeyHoldCallback(GLFW_KEY_S, [this] () { moveCamera(Vector2D<int>{0, -1}); });
        inputHandler->addKeyHoldCallback(GLFW_KEY_D, [this] () { moveCamera(Vector2D<int>{1, 0}); });

        inputHandler->addButtonPressCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { exitUi(); });
        inputHandler->addButtonReleaseCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { enterUi(); });

        inputHandler->addMouseMoveCallback([this] (double x, double y) { lookAround(x, -y); });
        inputHandler->addKeyPressCallback(GLFW_KEY_ESCAPE, [this] () { display->close(); });

        glfwSetKeyCallback(display->getWindow(), &InputHandler::registerKeys);
        glfwSetMouseButtonCallback(display->getWindow(), &InputHandler::registerButtons);
        glfwSetCursorPosCallback(display->getWindow(), &InputHandler::processMouseInput);

        display->addFloatSlider("Terrain", "Camera Speed", &camera->Speed, 1.f, 10.f);
        display->addIntSlider("Terrain", "Octaves", &generator->octaves, 1, 16);
        display->addFloatSlider("Terrain", "Mesh Height", &generator->meshHeight, 1, 640);
        display->addFloatSlider("Terrain", "Noise Scale", &generator->noiseScale, 0, 1000);
        display->addFloatSlider("Terrain", "Persistence", &generator->persistence, 0, 1);
        display->addFloatSlider("Terrain", "Lacunarity", &generator->lacunarity, 1, 64);
        display->addButton("Terrain", "Regenerate", [this] () { generator->generateAllChunks(); });

        display->initImGui();



        mainShader->use();

    }

    void enterUi() {
        InputHandler *inputHandler = InputHandler::instance();
        inputHandler->showCursor(display->getWindow());
    }

    void exitUi() {
        InputHandler *inputHandler = InputHandler::instance();
        inputHandler->hideCursor(display->getWindow());
    }

    void onDeactivate() {
        display->shutdownImGui();
    }


    void processInput() {
        InputHandler::instance()->processInput();
    }

    void lookAround(double x, double y) {
        camera->ProcessMouseInput(x, y, true);
    }

    void moveCamera(Vector2D<int> input) {
        GLFWwindow *window = display->getWindow();
        float deltaTime = display->getDeltaTime();
        int x = input.x;
        int y = input.y;

        if(x == -1) {
            camera->ProcessKeyboard(LEFT, deltaTime);
        }

        if(x == 1) {
            camera->ProcessKeyboard(RIGHT, deltaTime);
        }

        if(y == -1) {
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        }

        if(y == 1) {
            camera->ProcessKeyboard(FORWARD, deltaTime);
        }
    }

    void update(float deltaTime) {
        camera->UpdateShader(mainShader);
    }

    void lateUpdate(float deltaTime) {

    }

    void draw(Display *display) override {
        mainShader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        mainShader->set_mat4_uniform("model", model);


        generator->renderChunk(0, 0);
    }
};





#endif // TERRAIN_GENERATION_SCENE