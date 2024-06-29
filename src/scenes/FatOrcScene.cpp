#ifndef FAT_ORC_SCENE
#define FAT_ORC_SCENE

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


#include "../utils/Scene.h"

// GLM Imports
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class FatOrcScene : virtual public Scene {
	Camera *camera;
	Shader mainShader;

	Model ourModel = Model(ROOT_DIR"/assets/models/FatTroll.obj");

	int scaleLocation;
	int modelLocation;

	glm::mat4 model = glm::mat4(1.0f);

public:
	Display *display;

	FatOrcScene(Display *_display) {
		display = _display;
	}

	void onCreate() {
		mainShader = Shader(ROOT_DIR"/assets/shaders/trollVertex.glsl", ROOT_DIR"/assets/shaders/trollFragment.glsl");
		mainShader.use();

		mainShader.addCamera(camera);

		scaleLocation = mainShader.getUniformLocation("scale");
		modelLocation = mainShader.getUniformLocation("model");
		Shader::setFloat(scaleLocation, 1.0f);
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
	
		display->initImGui();

		directionalLight = DirectionalLight(
			glm::vec3(1.0, -0.5, 0.2),  // Direction
			glm::vec3(0.8, 0.8, 0.8),   // Ambient
			glm::vec3(0.3, 0.3, 0.3),   // Diffuse
			glm::vec3(0.5, 0.5, 0.5)    // Specular
		);

		mainShader.use();

		mainShader.addDirectionalLight(directionalLight);
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
		mainShader.updateCamera();
	}

	void lateUpdate(float deltaTime) {

	}

	void draw(Display *display) override {
		mainShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		uint modelLocation = mainShader.getUniformLocation("model");
		Shader::setMat4(modelLocation, model);

		ourModel.Draw(mainShader);
	}
};

#endif // FAT_ORC_SCENE