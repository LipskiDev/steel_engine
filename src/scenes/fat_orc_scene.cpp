#ifndef FAT_ORC_SCENE
#define FAT_ORC_SCENE

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


#include "../utils/scene.h"

// GLM Imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class FatOrcScene : virtual public Scene {
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<ShaderProgram> main_shader_;
	std::unique_ptr<Model> fat_troll_model_ = make_unique<Model>(ROOT_DIR"/assets/models/FatTroll.obj");
public:
	Display *display_;

	FatOrcScene(Display *window) {
		display_ = window;
	}

	void OnCreate() {
		main_shader_ = make_unique<ShaderProgram>(std::initializer_list<std::pair<std::string_view, Shader::Type>> {
			{ROOT_DIR"/assets/shaders/troll_vertex.glsl", Shader::Type::Vertex},
			{ROOT_DIR"/assets/shaders/troll_fragment.glsl", Shader::Type::Fragment}
		});;
		main_shader_->Use();

		camera_ = make_unique<Camera>(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 90.f, 1920.f / 1080.f, 1.f, 1000.f);
	}

	void OnDestroy() {

	}

	void OnActivate() {
		InputHandler *input_handler_ = InputHandler::Instance();


		input_handler_->AddKeyHoldCallback(GLFW_KEY_W, [this] () { MoveCamera(Vector2D<int>{0, 1}); });
		input_handler_->AddKeyHoldCallback(GLFW_KEY_A, [this] () { MoveCamera(Vector2D<int>{-1, 0}); });
		input_handler_->AddKeyHoldCallback(GLFW_KEY_S, [this] () { MoveCamera(Vector2D<int>{0, -1}); });
		input_handler_->AddKeyHoldCallback(GLFW_KEY_D, [this] () { MoveCamera(Vector2D<int>{1, 0}); });

		input_handler_->AddButtonPressCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { ExitUi(); });
		input_handler_->AddButtonReleaseCallback(GLFW_MOUSE_BUTTON_RIGHT, [this] () { EnterUi(); });

		input_handler_->AddMouseMoveCallback([this] (double x, double y) { LookAround(x, -y); });
		input_handler_->AddKeyPressCallback(GLFW_KEY_ESCAPE, [this] () { display_->Close(); });

		glfwSetKeyCallback(display_->GetWindow(), &InputHandler::RegisterKeys);
		glfwSetMouseButtonCallback(display_->GetWindow(), &InputHandler::RegisterButtons);
		glfwSetCursorPosCallback(display_->GetWindow(), &InputHandler::ProcessMouseInput);
	
		display_->InitImGui();

		main_shader_->Use();
	}

	void EnterUi() {
		InputHandler *input_handler_ = InputHandler::Instance();
		input_handler_->ShowCursor(display_->GetWindow());
	}

	void ExitUi() {
		InputHandler *input_handler_ = InputHandler::Instance();
		input_handler_->HideCursor(display_->GetWindow());
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
		float delta_time = display_->GetDeltaTime();
		int x = input.x;
		int y = input.y;

		if(x == -1) {
			camera_->ProcessKeyboard(LEFT, delta_time);
		}

		if(x == 1) {
			camera_->ProcessKeyboard(RIGHT, delta_time);
		}

		if(y == -1) {
			camera_->ProcessKeyboard(BACKWARD, delta_time);
		}

		if(y == 1) {
			camera_->ProcessKeyboard(FORWARD, delta_time);
		}
	}

	void Update(float delta_time) {
		camera_->UpdateShader(main_shader_);
	}

	void LateUpdate(float delta_time) {

	}

	void Draw(Display *display) override {
		main_shader_->Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		main_shader_->SetMat4Uniform("model", model);

		fat_troll_model_->Draw(main_shader_);
	}
};

#endif // FAT_ORC_SCENE