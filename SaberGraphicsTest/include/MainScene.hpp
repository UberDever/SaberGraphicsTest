#ifndef SABERGRAPHICSTEST_MAINSCENE_H
#define SABERGRAPHICSTEST_MAINSCENE_H

#include "app.hpp"
#include "renderer.hpp"
#include "geometry.hpp"

class MainScene : public Scene
{
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); // generic API callback for pass-through class data
	static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);

	static constexpr auto vertexShaderPath = "shaders/MainScene.vs";
	static constexpr auto fragmentShaderPath = "shaders/MainScene.fs";

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods); // class friendly callback
	void onMouse(GLFWwindow* window, double xpos, double ypos);

	Camera mCamera;
	Renderer mRenderer;

	Model* cubeHardEdges;
	Model* cubeSmoothEdges;
	Model* cubeSmoothedByMe;

	glm::vec3 pLightPosition = { 0, 0, 0 };

	glm::vec3 pLightAttenuation = { 0.008, 0.03, 1 };
	glm::vec3 pLightAmbient = { 0.65, 0.65, 0.65 };
	glm::vec3 pLightDiffuse = { 0.8, 0.8, 0.8 };
	glm::vec3 pLightSpecular = { 1, 1, 1 };

	enum class Colors { Red = 0, Orange, Yellow, Green, LightBlue, Blue, Purple, Black, White };
	Colors pLightColor = Colors::White;
	std::vector<glm::vec3> pLightColors = { {1, 0, 0}, {0.99, 0.53, 0}, {1, 1, 0}, {0, 1, 0}, {.345, 0.8, 0.92}, {0, 0, 1}, {.38, 0.185, 0.42}, {0,0,0}, {1,1,1} };

public:
	void processEvents(GLFWwindow* window) override;
	void beginFrame() override;
	void update() override;
	void render() override;
	void endFrame() override;

	void setInputCallback(GLFWwindow* window) override
	{
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPosCallback(window, mousePosCallback);
	}

	MainScene();
};

#endif