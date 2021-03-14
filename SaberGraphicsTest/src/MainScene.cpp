#include "../include/MainScene.hpp"
#include "../Assignment/NormalSmoothing.hpp"

MainScene::MainScene() : mRenderer(Shader(vertexShaderPath, fragmentShaderPath))
{
	std::vector<std::string> samplersNames = { "u_material.diffuse", "u_material.specular" };
	mRenderer.mainShader().bind();
	mRenderer.mainShader().findSamplersLocation(samplersNames);

	Mesh lampMesh = createSphere(16, .1); //createCube(1, {1, 1, 1});//
	Model lamp(lampMesh);
	mRenderer.setLightModel(lamp);

	mRenderer.mainShader().set1f("u_material.shininess", 12.f);

	/*
	DirLight dirLight;
	dirLight.dir = { -0.2, -1, -0.5 };
	dirLight.ambient = { 0.15f, 0.15f, 0.15f };
	dirLight.diffuse = { 0.45f, 0.45f, 0.45f };
	dirLight.specular = { 0.85f, 0.85f, 0.85f };

	mRenderer.addDirLight(dirLight);
	*/

	{
		Mesh cubeHardEdgesMesh = obj_parse("meshes/CubeHardEdges.model");
		cubeHardEdges = new Model(cubeHardEdgesMesh);
		Texture diffuseMap("textures/pink.png");
		cubeHardEdges->addTexture(diffuseMap);
		Texture specularMap("textures/pink.png");
		cubeHardEdges->addTexture(specularMap);
	}

	{
		Mesh cubeSmoothEdgesMesh = obj_parse("meshes/CubeSmoothedBlender.model");
		cubeSmoothEdges = new Model(cubeSmoothEdgesMesh);
		Texture diffuseMap("textures/green.png");
		cubeSmoothEdges->addTexture(diffuseMap);
		Texture specularMap("textures/green.png");
		cubeSmoothEdges->addTexture(specularMap);
	}

	{
		Mesh cubeSmoothedByMeMesh = obj_parse("meshes/CubeHardEdges.model");
		smoothNormals<true>(&cubeSmoothedByMeMesh);
		cubeSmoothedByMe = new Model(cubeSmoothedByMeMesh);
		Texture diffuseMap("textures/blue.png");
		cubeSmoothedByMe->addTexture(diffuseMap);
		Texture specularMap("textures/blue.png");
		cubeSmoothedByMe->addTexture(specularMap);
	}

	{
		Mesh cylinderHardEdgesMesh = obj_parse("meshes/CylinderHardEdges.model");
		smoothNormals<false>(&cylinderHardEdgesMesh);
		cylinderHardEdges = new Model(cylinderHardEdgesMesh);
		Texture diffuseMap("textures/pink.png");
		cylinderHardEdges->addTexture(diffuseMap);
		Texture specularMap("textures/pink.png");
		cylinderHardEdges->addTexture(specularMap);
	}

	{
		Mesh cylinderSmoothEdgesMesh = obj_parse("meshes/CylinderSmoothedBlender.model");
		cylinderSmoothEdges = new Model(cylinderSmoothEdgesMesh);
		Texture diffuseMap("textures/blue.png");
		cylinderSmoothEdges->addTexture(diffuseMap);
		Texture specularMap("textures/blue.png");
		cylinderSmoothEdges->addTexture(specularMap);
	}

	{
		Mesh cylinderSmoothedByMeMesh = obj_parse("meshes/CylinderHardEdges.model");
		smoothNormals<true>(&cylinderSmoothedByMeMesh);
		cylinderSmoothedByMe = new Model(cylinderSmoothedByMeMesh);
		Texture diffuseMap("textures/green.png");
		cylinderSmoothedByMe->addTexture(diffuseMap);
		Texture specularMap("textures/green.png");
		cylinderSmoothedByMe->addTexture(specularMap);
	}

	mCamera.setProjection(glm::radians(90.0f), (float)App::instance().getW() / (float)App::instance().getH(), 0.1f, 100.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void MainScene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		App::instance().close();
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
	}
	((MainScene*)glfwGetWindowUserPointer(window))->onKey(window, key, scancode, action, mods);
}

void MainScene::onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void MainScene::mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	((MainScene*)glfwGetWindowUserPointer(window))->onMouse(window, xpos, ypos);
}

void MainScene::onMouse(GLFWwindow* window, double xpos, double ypos)
{
	glm::vec2 offset = App::instance().calculateRelMouse({ xpos, ypos });
	offset *= mCamera.sensitivity;
	mCamera.rotation += offset;

	static constexpr float safePI = M_PI_2 - 0.1745f;
	if (mCamera.rotation.y > safePI)
		mCamera.rotation.y = safePI;
	else if (mCamera.rotation.y < -safePI)
		mCamera.rotation.y = -safePI;
	if (mCamera.rotation.x < 0)
		mCamera.rotation.x = 2 * M_PI;
	else if (mCamera.rotation.x > 2 * M_PI)
		mCamera.rotation.x = 0;

	mCamera.forward = glm::rotateY(glm::rotateX(glm::vec3(0, 0, 1), mCamera.rotation.y), mCamera.rotation.x);
	mCamera.up = glm::rotateY(glm::rotateX(glm::vec3(0, 1, 0), mCamera.rotation.y), mCamera.rotation.x);
	mCamera.right = glm::rotateY(glm::rotateX(glm::vec3(1, 0, 0), mCamera.rotation.y), mCamera.rotation.x);
}

void MainScene::processEvents(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mCamera.position += mCamera.forward * mCamera.speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mCamera.position -= mCamera.forward * mCamera.speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mCamera.position += mCamera.right * mCamera.speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mCamera.position -= mCamera.right * mCamera.speed;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pLightPosition += mCamera.up * .05f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pLightPosition -= mCamera.up * .05f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pLightPosition += mCamera.right * .05f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pLightPosition -= mCamera.right * .05f;
}

void MainScene::beginFrame()
{
	//glClearColor((float)0x22 / 0xFF, (float)0x22 / 0xFF, (float)0x22 / 0xFF, 1.0f);
	glClearColor((float)1, (float)1, (float)1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainScene::update()
{

}

void MainScene::render()
{
	mCamera.setView();

	mRenderer.lightShader().bind();
	mRenderer.lightShader().setM4(Renderer::defaultProjName, mCamera.projection);
	mRenderer.lightShader().setM4(Renderer::defaultViewName, mCamera.view);

	std::vector<glm::vec3> lightColors;

	mRenderer.addPointLightInstance(pLightPosition);
	lightColors.push_back(pLightColors[(int)Colors::Black]);

	mRenderer.renderLight(lightColors);

	mRenderer.mainShader().bind();
	mRenderer.mainShader().setV3("u_viewPos", mCamera.position);
	mRenderer.mainShader().setM4(Renderer::defaultProjName, mCamera.projection);
	mRenderer.mainShader().setM4(Renderer::defaultViewName, mCamera.view);

	PointLight pointLight;
	pointLight.pos = pLightPosition;
	pointLight.attenuation = pLightAttenuation;
	pointLight.ambient = pLightColors[(int)pLightColor] * pLightAmbient;
	pointLight.diffuse = pLightColors[(int)pLightColor] * pLightDiffuse;
	pointLight.specular = pLightColors[(int)pLightColor] * pLightSpecular;
	mRenderer.addPointLight(pointLight);
	mRenderer.add(*cubeHardEdges, glm::vec3(-3, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	mRenderer.add(*cubeSmoothEdges, glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	mRenderer.add(*cubeSmoothedByMe, glm::vec3(3, 0, 5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	mRenderer.add(*cylinderHardEdges, glm::vec3(-3, 0, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	mRenderer.add(*cylinderSmoothEdges, glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	mRenderer.add(*cylinderSmoothedByMe, glm::vec3(3, 0, -5), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	mRenderer.mainShader().set1i("u_pLightCount", mRenderer.getPointLightCount());

	mRenderer.render();

	mRenderer.removeAll();
	mRenderer.clearPointLights();
}

void MainScene::endFrame()
{
	App::instance().swapBuffers();
}
