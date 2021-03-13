#include "../include/app.hpp"

App::App(int w, int h) : mWindowH(h), mWindowW(w), mCurScene(0), mLastMouseCoords(w / 2, h / 2)
{
	srand(time(0));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	mWindow = glfwCreateWindow(w, h, "Lab2", NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::logic_error("Failed to initialize GLAD");
	}

	printf("OpenGL loaded\n");
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int App::exec()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
		mScenes[mCurScene]->processEvents(mWindow);
		mScenes[mCurScene]->beginFrame();
		mScenes[mCurScene]->update();
		mScenes[mCurScene]->render();
		mScenes[mCurScene]->endFrame();
	}

	glfwTerminate();
	return 0;
}
