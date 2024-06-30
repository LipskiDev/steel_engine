#ifndef TERRAIN_GENERATION_SCENE
#define TERRAIN_GENERATION_SCENE

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "../utils/scene.h"

#include "../math/vector.h"

// GLM Imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../input/input_handler.h"


#include "../terrain/perlin_noise_chunk_generator.h"
#include "../terrain/terrain.h"

#include "../utils/shader.h"
#include "../ui/display.h"
#include "../utils/scene.h"
#include "../objects/camera.h"

#include "../model/model.h"

#include "../config.h"

#include <vector>

class TerrainGenerationScene : virtual public Scene {

private:
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<ShaderProgram> main_shader_;
    std::unique_ptr<PerlinNoiseChunkGenerator> generator_;
public:
    Display *display_;

    TerrainGenerationScene(Display *window) {
        display_ = window;
    }

    void OnCreate() {
        main_shader_ = make_unique<ShaderProgram>(std::initializer_list<std::pair<std::string_view, Shader::Type>> {
            {ROOT_DIR"/assets/shaders/terrain_vertex.glsl", Shader::Type::Vertex},
            {ROOT_DIR"/assets/shaders/terrain_fragment.glsl", Shader::Type::Fragment}
        });
        main_shader_->Use();

        generator_ = make_unique<PerlinNoiseChunkGenerator>();

        camera_ = make_unique<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 90.f, 1920.f / 1080.f, 1.f, 1000.f);

        main_shader_->SetFloatUniform("waterHeight", generator_->GetWaterHeight());
        main_shader_->SetFloatUniform("meshHeight", generator_->GetMeshHeight());
        main_shader_->SetFloatUniform("scale", 1.0f);
    }

    void OnDestroy() {

    }

    void OnActivate() {
        InputHandler *input_handler = InputHandler::Instance();

        input_handler->AddKeyHoldCallback(GLFW_KEY_W, [this] () { MoveCamera(Vector2D<int>{0, 1}); });
        input_handler->AddKeyHoldCallback(GLFW_KEY_A, [this] () { MoveCamera(Vector2D<int>{-1, 0}); });
        input_handler->AddKeyHoldCallback(GLFW_KEY_S, [this] () { MoveCamera(Vector2D<int>{0, -1}); });
        input_handler->AddKeyHoldCallback(GLFW_KEY_D, [this] () { MoveCamera(Vector2D<int>{1, 0}); });
        
        input_handler->AddButtonPressCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { ExitUi(); });
        input_handler->AddButtonReleaseCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { EnterUi(); });
        
        input_handler->AddMouseMoveCallback([this] (double x, double y) { LookAround(x, -y); });
        input_handler->AddKeyPressCallback(GLFW_KEY_ESCAPE, [this] () { display_->Close(); });

        glfwSetKeyCallback(display_->GetWindow(), &InputHandler::RegisterKeys);
        glfwSetMouseButtonCallback(display_->GetWindow(), &InputHandler::RegisterButtons);
        glfwSetCursorPosCallback(display_->GetWindow(), &InputHandler::ProcessMouseInput);

        display_->AddFloatSlider("Terrain", "Camera Speed", &camera_->speed_, 1.f, 10.f);
        display_->AddIntSlider("Terrain", "Octaves", &generator_->octaves_, 1, 16);
        display_->AddFloatSlider("Terrain", "Mesh Height", &generator_->mesh_height_, 1, 640);
        display_->AddFloatSlider("Terrain", "Noise Scale", &generator_->noise_scale_, 0, 1000);
        display_->AddFloatSlider("Terrain", "Persistence", &generator_->persistence_, 0, 1);
        display_->AddFloatSlider("Terrain", "Lacunarity", &generator_->lacunarity_, 1, 64);
        display_->AddButton("Terrain", "Regenerate", [this] () { generator_->GenerateAllChunks(); });

        display_->InitImGui();

        main_shader_->Use();

    }

    void EnterUi() {
        InputHandler *input_handler = InputHandler::Instance();
        input_handler->ShowCursor(display_->GetWindow());
    }

    void ExitUi() {
        InputHandler *input_handler = InputHandler::Instance();
        input_handler->HideCursor(display_->GetWindow());
    }

    void OnDeactivate() {
        display_->ShutdownImGui();
    }


    void ProcessInput() {
        InputHandler::Instance()->ProcessInput();
    }

    void LookAround(double x, double y) {
        camera_->ProcessMouseInput(x, y, true);
    }

    void MoveCamera(Vector2D<int> input) {
        GLFWwindow *window = display_->GetWindow();
        float deltaTime = display_->GetDeltaTime();
        int x = input.x;
        int y = input.y;

        if(x == -1) {
            camera_->ProcessKeyboard(LEFT, deltaTime);
        }

        if(x == 1) {
            camera_->ProcessKeyboard(RIGHT, deltaTime);
        }

        if(y == -1) {
            camera_->ProcessKeyboard(BACKWARD, deltaTime);
        }

        if(y == 1) {
            camera_->ProcessKeyboard(FORWARD, deltaTime);
        }
    }

    void Update(float deltaTime) {
        camera_->UpdateShader(main_shader_);
    }

    void LateUpdate(float deltaTime) {

    }

    void Draw(Display *display) override {
        main_shader_->Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        main_shader_->SetMat4Uniform("model", model);

        generator_->RenderChunk(0, 0);
    }
};
#endif // TERRAIN_GENERATION_SCENE